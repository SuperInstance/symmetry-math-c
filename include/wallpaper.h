#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <stdbool.h>

/* Wallpaper group types (17 wallpaper groups) */
typedef enum {
    SM_WP_NONE = 0,
    SM_WP_P1,      /* 1: no symmetry */
    SM_WP_PG,      /* 2: glide only */
    SM_WP_PM,      /* 3: mirror only */
    SM_WP_CM,      /* 4: mirror + glide */
    SM_WP_P2,      /* 5: 2-fold rotation */
    SM_WP_PGG,     /* 6: 2-fold + glide */
    SM_WP_PMG,     /* 7: 2-fold + mirror + glide */
    SM_WP_PMM,     /* 8: mirrors perpendicular */
    SM_WP_CMM,     /* 9: mirrors at angles */
    SM_WP_P3,      /* 10: 3-fold rotation */
    SM_WP_P31M,    /* 11 */
    SM_WP_P3M1,    /* 12 */
    SM_WP_P4,      /* 13: 4-fold rotation */
    SM_WP_P4G,     /* 14 */
    SM_WP_P4M,     /* 15 */
    SM_WP_P6,      /* 16: 6-fold rotation */
    SM_WP_P6M      /* 17: full hexagonal */
} sm_wallpaper_group;

/* Axis info for reflection detection */
typedef struct {
    bool   found;
    double angle;      /* axis angle in radians */
    double offset;     /* offset from center */
} sm_axis_info;

/* Classify a 2D pattern into a wallpaper group */
sm_wallpaper_group classify_pattern(const double *pattern, int width, int height);

/* Detect rotational symmetry of given order */
bool detect_rotational_symmetry(const double *pattern, int width, int height, int order);

/* Detect a reflection axis */
sm_axis_info detect_reflection_axis(const double *pattern, int width, int height);

#endif /* WALLPAPER_H */
