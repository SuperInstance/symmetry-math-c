#include "orbit.h"
#include <stdlib.h>
#include <string.h>

/* Helper: check if element is in sorted array */
static bool in_sorted(const int *arr, int count, int val) {
    for (int i = 0; i < count; i++) {
        if (arr[i] == val) return true;
        if (arr[i] > val) return false;
    }
    return false;
}

/* Helper: insert into sorted array */
static int insert_sorted(int *arr, int *count, int val) {
    if (in_sorted(arr, *count, val)) return 0;
    int pos = *count;
    for (int i = 0; i < *count; i++) {
        if (arr[i] > val) { pos = i; break; }
    }
    memmove(&arr[pos + 1], &arr[pos], sizeof(int) * (size_t)(*count - pos));
    arr[pos] = val;
    (*count)++;
    return 1;
}

sm_orbit orbit_compute(sm_arena *a, const sm_group *g, int element) {
    sm_orbit o;
    o.capacity = g->order;
    o.elements = (int *)sm_arena_alloc(a, sizeof(int) * (size_t)o.capacity);
    o.count = 0;

    /* BFS-style: apply all group elements to the starting element */
    int queue[256];
    int qstart = 0, qend = 0;
    queue[qend++] = element;
    insert_sorted(o.elements, &o.count, element);

    while (qstart < qend) {
        int current = queue[qstart++];
        for (int i = 0; i < g->order; i++) {
            int result = group_compose(g, i, current);
            if (!in_sorted(o.elements, o.count, result)) {
                insert_sorted(o.elements, &o.count, result);
                if (qend < 256) queue[qend++] = result;
            }
        }
    }
    return o;
}

sm_stabilizer stabilizer_compute(sm_arena *a, const sm_group *g, int element) {
    sm_stabilizer s;
    s.elements = (int *)sm_arena_alloc(a, sizeof(int) * (size_t)g->order);
    s.count = 0;

    /* Find all group elements that fix 'element' */
    for (int i = 0; i < g->order; i++) {
        if (group_compose(g, i, element) == element) {
            s.elements[s.count++] = i;
        }
    }
    return s;
}

long burnside_lemma(int group_size, const long *fixed_counts) {
    if (group_size <= 0 || !fixed_counts) return 0;
    long total = 0;
    for (int i = 0; i < group_size; i++) {
        total += fixed_counts[i];
    }
    return total / group_size;
}
