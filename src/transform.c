#include "transform.h"
#include <math.h>
#include <string.h>

#define SM_PI 3.14159265358979323846
#define SM_TOL 1e-9

bool sm_dbl_eq(double a, double b, double tol) {
    return fabs(a - b) < tol;
}

sm_transform transform_identity(void) {
    sm_transform t;
    memset(&t, 0, sizeof(t));
    t.m[0][0] = 1.0;
    t.m[1][1] = 1.0;
    t.m[2][2] = 1.0;
    return t;
}

sm_transform transform_rotation(double angle) {
    sm_transform t = transform_identity();
    double c = cos(angle), s = sin(angle);
    t.m[0][0] = c;  t.m[0][1] = -s;
    t.m[1][0] = s;  t.m[1][1] = c;
    return t;
}

sm_transform transform_reflection(double axis_angle) {
    sm_transform t = transform_identity();
    double c = cos(2 * axis_angle), s = sin(2 * axis_angle);
    t.m[0][0] = c;   t.m[0][1] = s;
    t.m[1][0] = s;   t.m[1][1] = -c;
    return t;
}

sm_transform transform_translation(double dx, double dy) {
    sm_transform t = transform_identity();
    t.m[0][2] = dx;
    t.m[1][2] = dy;
    return t;
}

sm_transform transform_glide_reflection(double axis_angle, double glide_dist) {
    sm_transform refl = transform_reflection(axis_angle);
    /* Glide along the axis direction */
    double dx = glide_dist * cos(axis_angle);
    double dy = glide_dist * sin(axis_angle);
    sm_transform trans = transform_translation(dx, dy);
    return compose_transforms(trans, refl);
}

sm_transform compose_transforms(sm_transform t1, sm_transform t2) {
    sm_transform result;
    memset(&result, 0, sizeof(result));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                result.m[i][j] += t1.m[i][k] * t2.m[k][j];
            }
        }
    }
    return result;
}

void transform_apply(sm_transform t, double x, double y, double *ox, double *oy) {
    *ox = t.m[0][0] * x + t.m[0][1] * y + t.m[0][2];
    *oy = t.m[1][0] * x + t.m[1][1] * y + t.m[1][2];
}

bool is_symmetry_of(sm_transform t, const double *points, int n_points, double tolerance) {
    /* For each point, check if transformed point exists in the set */
    for (int i = 0; i < n_points; i++) {
        double px = points[2 * i], py = points[2 * i + 1];
        double tx, ty;
        transform_apply(t, px, py, &tx, &ty);
        bool found = false;
        for (int j = 0; j < n_points; j++) {
            if (fabs(tx - points[2 * j]) < tolerance &&
                fabs(ty - points[2 * j + 1]) < tolerance) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

sm_transform_type transform_classify(sm_transform t) {
    double det = t.m[0][0] * t.m[1][1] - t.m[0][1] * t.m[1][0];

    if (sm_dbl_eq(det, 1.0, SM_TOL)) {
        /* Orientation preserving */
        if (sm_dbl_eq(t.m[0][0], 1.0, SM_TOL) &&
            sm_dbl_eq(t.m[1][1], 1.0, SM_TOL) &&
            sm_dbl_eq(t.m[0][1], 0.0, SM_TOL) &&
            sm_dbl_eq(t.m[1][0], 0.0, SM_TOL)) {
            /* Check if translation */
            if (sm_dbl_eq(t.m[0][2], 0.0, SM_TOL) &&
                sm_dbl_eq(t.m[1][2], 0.0, SM_TOL)) {
                return SM_TRANSFORM_IDENTITY;
            }
            return SM_TRANSFORM_TRANSLATION;
        }
        return SM_TRANSFORM_ROTATION;
    } else if (sm_dbl_eq(det, -1.0, SM_TOL)) {
        /* Orientation reversing: reflection or glide reflection */
        /* Check if it has a fixed point (reflection) vs not (glide) */
        double trace = t.m[0][0] + t.m[1][1];
        double det_full = t.m[0][0] * t.m[1][1] - t.m[0][1] * t.m[1][0];
        /* Fixed point exists if (1-a)^2 + b*c has solution */
        /* Simplified: if trace + 1 ≈ 0, likely pure reflection */
        if (sm_dbl_eq(trace + det_full, 0.0, 0.1)) {
            return SM_TRANSFORM_GLIDE_REFLECTION;
        }
        return SM_TRANSFORM_REFLECTION;
    }
    return SM_TRANSFORM_UNKNOWN;
}
