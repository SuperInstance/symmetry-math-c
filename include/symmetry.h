#ifndef SYMMETRY_H
#define SYMMETRY_H

#include <stddef.h>
#include <stdbool.h>

/* Arena allocator */
typedef struct {
    char  *base;
    size_t used;
    size_t capacity;
} sm_arena;

void  sm_arena_init(sm_arena *a, void *buf, size_t cap);
void *sm_arena_alloc(sm_arena *a, size_t size);

/* Group operation signature: compose elements i,j in a group of order n */
typedef int (*sm_binary_op)(int i, int j, int order);

/* Group struct */
typedef struct {
    int             order;       /* number of elements (0..order-1) */
    sm_binary_op    op;          /* binary operation */
    int            *inverses;    /* inverse[i] = inverse of i, arena-allocated */
} sm_group;

/* Core group functions */
sm_group *group_create(sm_arena *a, int order, sm_binary_op op);
int       group_compose(const sm_group *g, int a, int b);
int       group_inverse(const sm_group *g, int elem);
int       group_order(const sm_group *g);
bool      group_is_abelian(const sm_group *g);

/* Convenience constructors — allocate groups from arena */
sm_group *cyclic_group(sm_arena *a, int n);
sm_group *symmetric_group(sm_arena *a, int n);
sm_group *dihedral_group(sm_arena *a, int n);

/* Permutation helpers */
int  perm_compose(const int *p1, const int *p2, int *out, int n);
bool perm_is_identity(const int *p, int n);
int  perm_inverse(const int *p, int *out, int n);

/* Symmetric group uses permutation arrays stored externally */
#define SM_MAX_PERM_DEGREE 8

typedef struct {
    int order;           /* n! */
    int degree;          /* n (S_n) */
    int perms[40320][SM_MAX_PERM_DEGREE]; /* up to 8! = 40320 perms */
    int inverses[40320];
} sm_symmetric_group;

sm_symmetric_group *symmetric_group_full(sm_arena *a, int n);
int sym_compose(const sm_symmetric_group *sg, int i, int j);

/* Dihedral group helpers */
typedef struct {
    int order;       /* 2n */
    int n;           /* n-gon */
    int inverses[256];
} sm_dihedral_group;

sm_dihedral_group *dihedral_group_full(sm_arena *a, int n);
int dih_compose(const sm_dihedral_group *dg, int i, int j);

#endif /* SYMMETRY_H */
