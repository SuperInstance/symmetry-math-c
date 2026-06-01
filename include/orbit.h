#ifndef ORBIT_H
#define ORBIT_H

#include <stdbool.h>
#include "symmetry.h"

/* Orbit: set of elements reachable from a starting element under group action */
typedef struct {
    int  *elements;   /* sorted, unique */
    int   count;
    int   capacity;
} sm_orbit;

/* Stabilizer: subgroup that fixes an element */
typedef struct {
    int  *elements;
    int   count;
} sm_stabilizer;

/* Compute orbit of element under group action */
sm_orbit orbit_compute(sm_arena *a, const sm_group *g, int element);

/* Compute stabilizer subgroup */
sm_stabilizer stabilizer_compute(sm_arena *a, const sm_group *g, int element);

/* Burnside's lemma: count distinct colorings */
/* group_size: |G|, fixed_counts: array of fixed-point counts per group element */
long burnside_lemma(int group_size, const long *fixed_counts);

#endif /* ORBIT_H */
