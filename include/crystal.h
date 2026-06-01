#ifndef CRYSTAL_H
#define CRYSTAL_H

#include <stdbool.h>

/* 32 crystallographic point groups numbered 1-32 */
typedef enum {
    SM_PG_TRICLINIC_1   = 1,   /* C1 */
    SM_PG_TRICLINIC_1B  = 2,   /* Ci */
    SM_PG_MONO_2        = 3,   /* C2 */
    SM_PG_MONO_M        = 4,   /* Cs */
    SM_PG_MONO_2M       = 5,   /* C2h */
    SM_PG_ORTHO_222     = 6,   /* D2 */
    SM_PG_ORTHO_MM2     = 7,   /* C2v */
    SM_PG_ORTHO_MMM     = 8,   /* D2h */
    SM_PG_TETRA_4       = 9,   /* C4 */
    SM_PG_TETRA_4B      = 10,  /* S4 */
    SM_PG_TETRA_4M      = 11,  /* C4h */
    SM_PG_TETRA_422     = 12,  /* D4 */
    SM_PG_TETRA_4MM     = 13,  /* C4v */
    SM_PG_TETRA_42M     = 14,  /* D2d */
    SM_PG_TETRA_MMM     = 15,  /* D4h */
    SM_PG_TRIGO_3       = 16,  /* C3 */
    SM_PG_TRIGO_3B      = 17,  /* C3i */
    SM_PG_TRIGO_32      = 18,  /* D3 */
    SM_PG_TRIGO_3M      = 19,  /* C3v */
    SM_PG_TRIGO_3M1     = 20,  /* D3d */
    SM_PG_HEXAG_6       = 21,  /* C6 */
    SM_PG_HEXAG_6B      = 22,  /* C3h */
    SM_PG_HEXAG_6M      = 23,  /* C6h */
    SM_PG_HEXAG_622     = 24,  /* D6 */
    SM_PG_HEXAG_6MM     = 25,  /* C6v */
    SM_PG_HEXAG_6M2     = 26,  /* D3h */
    SM_PG_HEXAG_MMM     = 27,  /* D6h */
    SM_PG_CUBIC_23      = 28,  /* T */
    SM_PG_CUBIC_M3      = 29,  /* Th */
    SM_PG_CUBIC_432     = 30,  /* O */
    SM_PG_CUBIC_43M     = 31,  /* Td */
    SM_PG_CUBIC_M3M     = 32   /* Oh */
} sm_point_group;

/* Laue classes (11 centrosymmetric point groups) */
typedef enum {
    SM_LAUE_TRICLINIC   = 1,   /* -1 */
    SM_LAUE_MONO        = 2,   /* 2/m */
    SM_LAUE_ORTHO       = 3,   /* mmm */
    SM_LAUE_TETRA       = 4,   /* 4/m, 4/mmm */
    SM_LAUE_TRIGO       = 5,   /* -3, -3m */
    SM_LAUE_HEXAG       = 6,   /* 6/m, 6/mmm */
    SM_LAUE_CUBIC       = 7    /* m-3, m-3m */
} sm_laue_class;

/* Determine point group from rotation and reflection counts */
sm_point_group point_group_from_symmetry(int n_rotations, int n_reflections,
                                          int max_rotation_order);

/* Get Laue class from point group */
sm_laue_class laue_class(sm_point_group pg);

/* Check if reflection (h,k,l) would be systematically absent */
bool systematic_absences(sm_point_group pg, int h, int k, int l);

#endif /* CRYSTAL_H */
