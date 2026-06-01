#include "symmetry.h"
#include <string.h>
#include <stdlib.h>

/* --- Arena --- */

void sm_arena_init(sm_arena *a, void *buf, size_t cap) {
    a->base = (char *)buf;
    a->used = 0;
    a->capacity = cap;
}

void *sm_arena_alloc(sm_arena *a, size_t size) {
    /* Align to 8 bytes */
    size_t aligned = (size + 7) & ~(size_t)7;
    if (a->used + aligned > a->capacity) return NULL;
    void *ptr = a->base + a->used;
    a->used += aligned;
    memset(ptr, 0, aligned);
    return ptr;
}

/* --- Cyclic group operation: Z/nZ addition --- */
static int cyclic_op(int i, int j, int order) {
    return (i + j) % order;
}

/* --- Core group functions --- */

sm_group *group_create(sm_arena *a, int order, sm_binary_op op) {
    sm_group *g = (sm_group *)sm_arena_alloc(a, sizeof(sm_group));
    if (!g) return NULL;
    g->order = order;
    g->op = op;
    g->inverses = (int *)sm_arena_alloc(a, sizeof(int) * (size_t)order);
    if (!g->inverses) return NULL;

    /* Compute inverses by brute force */
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            if (op(i, j, order) == 0) {
                g->inverses[i] = j;
                break;
            }
        }
    }
    return g;
}

int group_compose(const sm_group *g, int a, int b) {
    return g->op(a, b, g->order);
}

int group_inverse(const sm_group *g, int elem) {
    if (elem < 0 || elem >= g->order) return -1;
    return g->inverses[elem];
}

int group_order(const sm_group *g) {
    return g->order;
}

bool group_is_abelian(const sm_group *g) {
    for (int i = 0; i < g->order; i++) {
        for (int j = i + 1; j < g->order; j++) {
            if (g->op(i, j, g->order) != g->op(j, i, g->order))
                return false;
        }
    }
    return true;
}

sm_group *cyclic_group(sm_arena *a, int n) {
    return group_create(a, n, cyclic_op);
}

/* --- Permutation helpers --- */

int perm_compose(const int *p1, const int *p2, int *out, int n) {
    /* out[i] = p1[p2[i]] (apply p2 first, then p1) */
    for (int i = 0; i < n; i++) {
        out[i] = p1[p2[i]];
    }
    return 0;
}

bool perm_is_identity(const int *p, int n) {
    for (int i = 0; i < n; i++) {
        if (p[i] != i) return false;
    }
    return true;
}

int perm_inverse(const int *p, int *out, int n) {
    for (int i = 0; i < n; i++) {
        out[p[i]] = i;
    }
    return 0;
}

/* --- Symmetric group (factorial-based) --- */

static int factorial(int n) {
    int f = 1;
    for (int i = 2; i <= n; i++) f *= i;
    return f;
}

/* Heap's algorithm to generate all permutations */
static void generate_perms(int n, int perms[][SM_MAX_PERM_DEGREE]) {
    int perm[SM_MAX_PERM_DEGREE];
    for (int i = 0; i < n; i++) perm[i] = i;
    int idx = 0;
    /* Copy initial */
    memcpy(perms[idx++], perm, sizeof(int) * (size_t)n);

    int c[SM_MAX_PERM_DEGREE];
    memset(c, 0, sizeof(c));

    int i = 0;
    while (i < n) {
        if (c[i] < i) {
            if (i % 2 == 0) {
                int tmp = perm[0]; perm[0] = perm[i]; perm[i] = tmp;
            } else {
                int tmp = perm[c[i]]; perm[c[i]] = perm[i]; perm[i] = tmp;
            }
            memcpy(perms[idx++], perm, sizeof(int) * (size_t)n);
            c[i]++;
            i = 0;
        } else {
            c[i] = 0;
            i++;
        }
    }
}

static int find_perm_idx(const int perms[][SM_MAX_PERM_DEGREE], int n_perms, int degree, const int *target) {
    for (int i = 0; i < n_perms; i++) {
        bool match = true;
        for (int j = 0; j < degree; j++) {
            if (perms[i][j] != target[j]) { match = false; break; }
        }
        if (match) return i;
    }
    return -1;
}

sm_symmetric_group *symmetric_group_full(sm_arena *a, int n) {
    if (n < 1 || n > SM_MAX_PERM_DEGREE) return NULL;
    sm_symmetric_group *sg = (sm_symmetric_group *)sm_arena_alloc(a, sizeof(sm_symmetric_group));
    if (!sg) return NULL;
    sg->degree = n;
    sg->order = factorial(n);
    generate_perms(n, sg->perms);

    /* Compute inverses */
    for (int i = 0; i < sg->order; i++) {
        int inv[SM_MAX_PERM_DEGREE];
        perm_inverse(sg->perms[i], inv, n);
        sg->inverses[i] = find_perm_idx(sg->perms, sg->order, n, inv);
    }
    return sg;
}

int sym_compose(const sm_symmetric_group *sg, int i, int j) {
    int result[SM_MAX_PERM_DEGREE];
    perm_compose(sg->perms[i], sg->perms[j], result, sg->degree);
    return find_perm_idx(sg->perms, sg->order, sg->degree, result);
}

/* --- Dihedral group --- */
/* Elements: r^i (rotation) and r^i*s (reflection), i=0..n-1
 * Indices: 0..n-1 = rotations, n..2n-1 = reflections */

static int dihedral_op(int i, int j, int order) {
    int n = order / 2;
    int ri = i % n, si = i >= n ? 1 : 0;
    int rj = j % n, sj = j >= n ? 1 : 0;
    /* r^a s^b * r^c s^d = r^(a + (-1)^b * c) s^(b+d)
     * where exponent is mod n and s^2 = e */
    int new_r;
    int new_s = (si + sj) % 2;
    if (si == 0) {
        new_r = (ri + rj) % n;
    } else {
        new_r = (ri + (n - rj) % n) % n;
    }
    return new_s ? new_r + n : new_r;
}

sm_dihedral_group *dihedral_group_full(sm_arena *a, int n) {
    if (n < 1 || n > 128) return NULL;
    sm_dihedral_group *dg = (sm_dihedral_group *)sm_arena_alloc(a, sizeof(sm_dihedral_group));
    if (!dg) return NULL;
    dg->n = n;
    dg->order = 2 * n;
    /* Compute inverses */
    for (int i = 0; i < dg->order; i++) {
        for (int j = 0; j < dg->order; j++) {
            if (dihedral_op(i, j, dg->order) == 0) {
                dg->inverses[i] = j;
                break;
            }
        }
    }
    return dg;
}

int dih_compose(const sm_dihedral_group *dg, int i, int j) {
    return dihedral_op(i, j, dg->order);
}

/* Legacy symmetric_group/dihedral_group using sm_group */
sm_group *symmetric_group(sm_arena *a, int n) {
    /* For small n, use cyclic representation (not full permutation) */
    /* This is a simplified version — full version needs symmetric_group_full */
    if (n > 8) return NULL;
    /* For S_n with n>2, not abelian — we store as sm_group with dihedral-like op
     * For proper use, use symmetric_group_full */
    /* Fallback: treat as cyclic for n<=2 */
    if (n <= 2) return cyclic_group(a, n);
    /* For n>2, return NULL — user should use symmetric_group_full */
    return NULL;
}

sm_group *dihedral_group(sm_arena *a, int n) {
    return group_create(a, 2 * n, dihedral_op);
}
