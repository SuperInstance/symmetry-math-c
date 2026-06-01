# symmetry-math-c

C99 library for symmetry group mathematics — designed for edge/embedded devices.

Part of the [symmetry-math](https://github.com/SuperInstance/symmetry-math) family.

## Features

- **Group theory**: Cyclic, symmetric, and dihedral groups with composition, inverses, and Abelian checks
- **2D transforms**: Rotation, reflection, glide reflection, composition, and symmetry-of-shape detection
- **Wallpaper groups**: Pattern classification into the 17 wallpaper groups
- **Orbit-stabilizer**: Orbit computation, stabilizer subgroups, Burnside's lemma
- **Crystallography**: 32 point groups, 11 Laue classes, systematic absences
- **Arena allocator**: Zero malloc, single buffer allocation for embedded use

## Building

```bash
make          # Build static library
make test     # Build and run tests
make clean    # Remove build artifacts
```

## Usage

```c
#include "symmetry_api.h"

char buf[1024 * 1024];  // 1MB arena
sm_arena arena;
sm_arena_init(&arena, buf, sizeof(buf));

sm_group *z5 = cyclic_group(&arena, 5);
int result = group_compose(z5, 2, 3);  // 0

sm_transform rot90 = transform_rotation(M_PI / 2);
double x, y;
transform_apply(rot90, 1.0, 0.0, &x, &y);  // (0, 1)
```

## Architecture

| File | Description |
|------|-------------|
| `symmetry.h/.c` | Groups, arena allocator, cyclic/symmetric/dihedral groups |
| `transform.h/.c` | 2D transforms as 3×3 homogeneous matrices |
| `wallpaper.h/.c` | Pattern symmetry detection and wallpaper group classification |
| `orbit.h/.c` | Orbits, stabilizers, Burnside's lemma |
| `crystal.h/.c` | Crystallographic point groups, Laue classes, systematic absences |
| `symmetry_api.h/.c` | Unified API header |

## Requirements

- C99 compiler (gcc, clang, etc.)
- libm (math library)
- No other external dependencies

## License

MIT
