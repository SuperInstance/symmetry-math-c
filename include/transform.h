#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <stdbool.h>

/* 2D transform represented as 3x3 homogeneous matrix */
typedef struct {
    double m[3][3];
} sm_transform;

/* Transform type tags */
typedef enum {
    SM_TRANSFORM_IDENTITY = 0,
    SM_TRANSFORM_ROTATION,
    SM_TRANSFORM_REFLECTION,
    SM_TRANSFORM_GLIDE_REFLECTION,
    SM_TRANSFORM_TRANSLATION,
    SM_TRANSFORM_UNKNOWN
} sm_transform_type;

/* Constructors */
sm_transform transform_identity(void);
sm_transform transform_rotation(double angle_radians);
sm_transform transform_reflection(double axis_angle_radians);
sm_transform transform_translation(double dx, double dy);
sm_transform transform_glide_reflection(double axis_angle_radians, double glide_dist);

/* Composition */
sm_transform compose_transforms(sm_transform t1, sm_transform t2);

/* Apply transform to a 2D point */
void transform_apply(sm_transform t, double x, double y, double *ox, double *oy);

/* Check if a transform is a symmetry of a set of points (within tolerance) */
bool is_symmetry_of(sm_transform t, const double *points, int n_points, double tolerance);

/* Classify a transform */
sm_transform_type transform_classify(sm_transform t);

/* Utility: approximate equality for doubles */
bool sm_dbl_eq(double a, double b, double tol);

#endif /* TRANSFORM_H */
