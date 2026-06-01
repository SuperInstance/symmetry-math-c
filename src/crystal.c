#include "crystal.h"

sm_laue_class laue_class(sm_point_group pg) {
    if (pg == SM_PG_TRICLINIC_1B) return SM_LAUE_TRICLINIC;
    if (pg >= SM_PG_MONO_2 && pg <= SM_PG_MONO_2M) return SM_LAUE_MONO;
    if (pg >= SM_PG_ORTHO_222 && pg <= SM_PG_ORTHO_MMM) return SM_LAUE_ORTHO;
    if (pg >= SM_PG_TETRA_4 && pg <= SM_PG_TETRA_MMM) return SM_LAUE_TETRA;
    if (pg >= SM_PG_TRIGO_3 && pg <= SM_PG_TRIGO_3M1) return SM_LAUE_TRIGO;
    if (pg >= SM_PG_HEXAG_6 && pg <= SM_PG_HEXAG_MMM) return SM_LAUE_HEXAG;
    if (pg >= SM_PG_CUBIC_23) return SM_LAUE_CUBIC;
    return SM_LAUE_TRICLINIC;
}

sm_point_group point_group_from_symmetry(int n_rotations, int n_reflections,
                                          int max_rotation_order) {
    (void)n_rotations;
    /* Simplified classification based on symmetry counts */
    if (max_rotation_order <= 1 && n_reflections == 0)
        return SM_PG_TRICLINIC_1;
    if (max_rotation_order <= 1 && n_reflections == 1)
        return SM_PG_MONO_M;
    if (max_rotation_order == 2 && n_reflections == 0)
        return SM_PG_MONO_2;
    if (max_rotation_order == 2 && n_reflections == 1)
        return SM_PG_MONO_2M;
    if (max_rotation_order == 2 && n_reflections >= 2)
        return SM_PG_ORTHO_MMM;
    if (max_rotation_order == 3 && n_reflections == 0)
        return SM_PG_TRIGO_3;
    if (max_rotation_order == 3 && n_reflections >= 1)
        return SM_PG_TRIGO_3M;
    if (max_rotation_order == 4 && n_reflections == 0)
        return SM_PG_TETRA_4;
    if (max_rotation_order == 4 && n_reflections >= 1)
        return SM_PG_TETRA_4MM;
    if (max_rotation_order == 6 && n_reflections == 0)
        return SM_PG_HEXAG_6;
    if (max_rotation_order == 6 && n_reflections >= 1)
        return SM_PG_HEXAG_6MM;
    return SM_PG_TRICLINIC_1;
}

bool systematic_absences(sm_point_group pg, int h, int k, int l) {
    /* Simplified systematic absence rules */
    /* General: if all indices zero, not absent */
    if (h == 0 && k == 0 && l == 0) return false;

    /* Body-centered (I): h+k+l must be even */
    if (pg == SM_PG_ORTHO_MMM || pg == SM_PG_TETRA_MMM || pg == SM_PG_CUBIC_M3M) {
        if ((h + k + l) % 2 != 0) return true;
    }

    /* Face-centered (F): h,k,l all odd or all even */
    if (pg == SM_PG_CUBIC_M3M) {
        int parity = (h % 2) + (k % 2) + (l % 2);
        if (parity != 0 && parity != 3) return true;
    }

    /* Hexagonal: for some groups, l must be even */
    if (pg >= SM_PG_TRIGO_3 && pg <= SM_PG_HEXAG_MMM) {
        /* Simplified rule */
        if (l != 0 && (l % 2 != 0) && pg == SM_PG_HEXAG_6M) return true;
    }

    return false;
}
