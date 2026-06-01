# symmetry-math-c

> Mathematics of symmetry groups in C — group theory, 2D transforms, wallpaper groups, crystallography, and Burnside's lemma.

## What This Does

`symmetry-math-c` brings group theory and symmetry operations to C. Build cyclic, symmetric, and dihedral groups; compute orbits and stabilizers; apply 2D affine transforms; classify patterns into the 17 wallpaper groups; and access crystallographic point group data. Uses arena allocation. Use it for generative art, crystallography, game development, or high-performance geometry.

## The Cultural Root

See `symmetry-math` (PyPI) for the full cultural background. Symmetry groups formalize the universal human recognition of pattern and balance.

## Install

```bash
git clone https://github.com/SuperInstance/symmetry-math-c.git
cd symmetry-math-c
make
```

## Quick Start

```c
#include "symmetry_api.h"

int main() {
    char buf[65536];
    sm_arena arena;
    sm_arena_init(&arena, buf, sizeof(buf));

    // Build groups
    sm_group *c4 = sm_cyclic_group(&arena, 4);
    sm_group *s3 = sm_symmetric_group(&arena, 3);
    sm_group *d6 = sm_dihedral_group(&arena, 6);

    printf("C4 order: %d\n", group_order(c4));
    printf("S3 order: %d\n", group_order(s3));

    // Group operations
    int product = group_compose(c4, 1, 2);
    int inv = group_inverse(c4, 3);

    // 2D transforms
    sm_transform R = sm_rotation(M_PI / 4);
    sm_transform M = sm_reflection(M_PI / 4);
    double ox, oy;
    transform_apply(R, 1.0, 0.0, &ox, &oy);

    // Wallpaper classification
    // (requires point set and lattice vectors)

    sm_arena_reset(&arena);
    return 0;
}
```

## API Reference

### Arena
- `void sm_arena_init(sm_arena *a, void *buf, size_t cap)`
- `void *sm_arena_alloc(sm_arena *a, size_t size)`

### Groups (`symmetry.h`)
- `sm_group *sm_cyclic_group(sm_arena *a, int n)`
- `sm_group *sm_symmetric_group(sm_arena *a, int n)`
- `sm_group *sm_dihedral_group(sm_arena *a, int n)`
- `int group_compose(const sm_group *g, int a, int b)`
- `int group_inverse(const sm_group *g, int elem)`
- `int group_order(const sm_group *g)`

### Permutations
- `int perm_compose(const int *p1, const int *p2, int *out, int n)`
- `int perm_inverse(const int *p, int *out, int n)`

### Transforms (`transform.h`)
- `sm_transform sm_rotation(double angle)`
- `sm_transform sm_reflection(double axis_angle)`
- `void transform_apply(sm_transform t, double x, double y, double *ox, double *oy)`

### Orbit & Wallpaper (`orbit.h`, `wallpaper.h`, `crystal.h`)
- Orbit and stabilizer computation
- Wallpaper group classification
- Crystallographic point group data

## License

MIT
