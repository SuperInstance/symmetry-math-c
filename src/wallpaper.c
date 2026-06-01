#include "wallpaper.h"
#include "transform.h"
#include <math.h>
#include <string.h>

#define SM_PI 3.14159265358979323846

/* Internal: check if pattern (as 2D grayscale) has rotational symmetry */
static bool check_rot_sym(const double *pattern, int width, int height,
                           double cx, double cy, int order, double tol) {
    double angle = 2.0 * SM_PI / order;
    sm_transform rot = transform_rotation(angle);
    int n = width * height;
    for (int attempt = 0; attempt < 8; attempt++) {
        bool all_match = true;
        /* Sample points from pattern */
        for (int i = 0; i < n; i += 3) {
            int x = i % width;
            int y = i / width;
            double px = x - cx, py = y - cy;
            double rx, ry;
            transform_apply(rot, px, py, &rx, &ry);
            int nx = (int)round(rx + cx);
            int ny = (int)round(ry + cy);
            if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
                all_match = false;
                break;
            }
            if (fabs(pattern[i] - pattern[ny * width + nx]) > tol) {
                all_match = false;
                break;
            }
        }
        if (all_match) return true;
    }
    return false;
}

bool detect_rotational_symmetry(const double *pattern, int width, int height, int order) {
    if (!pattern || width <= 0 || height <= 0 || order < 1) return false;
    double cx = width / 2.0;
    double cy = height / 2.0;
    return check_rot_sym(pattern, width, height, cx, cy, order, 0.1);
}

sm_axis_info detect_reflection_axis(const double *pattern, int width, int height) {
    sm_axis_info info = {false, 0.0, 0.0};
    if (!pattern || width <= 0 || height <= 0) return info;

    double cx = width / 2.0;
    double cy = height / 2.0;

    /* Check horizontal, vertical, and diagonal axes */
    double angles[] = {0.0, SM_PI / 2, SM_PI / 4, 3 * SM_PI / 4};
    int n_angles = 4;

    for (int a = 0; a < n_angles; a++) {
        sm_transform refl = transform_reflection(angles[a]);
        bool match = true;
        int samples = width * height;
        for (int i = 0; i < samples; i += 3) {
            int x = i % width;
            int y = i / width;
            double px = x - cx, py = y - cy;
            double rx, ry;
            transform_apply(refl, px, py, &rx, &ry);
            int nx = (int)round(rx + cx);
            int ny = (int)round(ry + cy);
            if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;
            if (fabs(pattern[i] - pattern[ny * width + nx]) > 0.1) {
                match = false;
                break;
            }
        }
        if (match) {
            info.found = true;
            info.angle = angles[a];
            info.offset = 0.0;
            return info;
        }
    }
    return info;
}

sm_wallpaper_group classify_pattern(const double *pattern, int width, int height) {
    if (!pattern || width <= 0 || height <= 0) return SM_WP_NONE;

    bool rot2 = detect_rotational_symmetry(pattern, width, height, 2);
    bool rot3 = detect_rotational_symmetry(pattern, width, height, 3);
    bool rot4 = detect_rotational_symmetry(pattern, width, height, 4);
    bool rot6 = detect_rotational_symmetry(pattern, width, height, 6);

    sm_axis_info axis = detect_reflection_axis(pattern, width, height);
    bool has_reflection = axis.found;

    if (rot6 && has_reflection) return SM_WP_P6M;
    if (rot6) return SM_WP_P6;
    if (rot4 && has_reflection) return SM_WP_P4M;
    if (rot4) return SM_WP_P4;
    if (rot3 && has_reflection) return SM_WP_P3M1;
    if (rot3) return SM_WP_P3;
    if (rot2 && has_reflection) return SM_WP_PMM;
    if (rot2) return SM_WP_P2;
    if (has_reflection) return SM_WP_PM;
    return SM_WP_P1;
}
