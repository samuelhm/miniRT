# AGENTS.md — AI Agent Instructions for RT (Ray Tracer)

## Project Overview

**RT** is a Monte Carlo path tracer written in C, part of the 42 school curriculum.  
It renders 3D scenes defined in `.rt` scene files using physically-based ray tracing.

**Subject:** `en.subject.md` (version 4.1)  
**Language:** C (C23 compatible, use modern features)  
**Binary:** `rt`  
**Build:** `make` (release, `-O3 -flto`), `make debug` (sanitizers)  
**External libs:** MLX42 (window/pixels), libvector (math), libft (being removed)

---

## How the Project Works

### Architecture Flow
```
main.c
  ├── init_data()           → allocates t_data struct
  ├── validate_args_and_open() → opens .rt file
  ├── parse()               → reads .rt, builds object/light/camera lists
  ├── init_mlx()            → creates window + image buffer
  ├── init_all()            → precomputes per-object ray data
  └── mlx_loop()            → event loop
       ├── render_fast()    → low-res preview + console UI
       ├── render_one()     → full path trace (1 sample)
       └── update_render()  → progressive accumulation
```

### Key Data Structures (`inc/data.h`)
- **`t_data`** — master struct: objects, lights, camera, MLX context, render state
- **`t_obj`** — scene object: type, position, axis, color, material, precomputed calcs
- **`t_cam`** — camera: position, FOV, orientation axis, DOF parameters
- **`t_material`** — material: type enum, reflectivity, roughness, textures, bump maps
- **`t_calcs`** — per-object cached math (radius², axis projections, refraction indices...)

### Object Types (enum, `data.h`)
`SP=0` sphere, `PL=1` plane, `CY=2` cylinder, `CAP=3` end cap, `CO=6` cone, `CU=7` cube, `SIDE=8` cube face

### Material Types (enum, `data.h`)
`MT=0` metallic, `GL=1` glass, `MR=2` mirror, `CB=3` checkerboard, `EM=4` emission, `BM=5` bump map, `TX=6` texture

### Ray Tracing Pipeline
1. **Ray generation** (`init_rays.c`): creates ray grid from camera/viewport
2. **Intersection** (`obj/*.c`): `find_closest()` tests all objects, returns closest hit
3. **Lighting** (`illumination.c`): ambient + diffuse + specular per light source
4. **Materials** (`materials.c`): glass (Fresnel), mirror, metallic, diffuse
5. **Path tracing** (`calcs.c`): recursive bounces up to `MAX_DEPTH=5`

### Scene File Format (`.rt`)
```
A    br r,g,b                    # ambient light
C    x,y,z nx,ny,nz fov [fd] [ap] # camera (fd=focus dist, ap=aperture)
L    x,y,z br r,g,b              # point light
sp   x,y,z diameter r,g,b [mat...] # sphere
pl   x,y,z nx,ny,nz r,g,b [mat...] # plane
cy   x,y,z nx,ny,nz d h r,g,b [mat...] # cylinder (d=diameter, h=height)
co   x,y,z nx,ny,nz d h r,g,b [mat...] # cone
cu   x,y,z nx,ny,nz w h d r,g,b [mat...] # cube (w,h,d dimensions)
#    comment
```
Material suffixes: `mt` metallic, `gl` glass, `mr` mirror, `cb scale r,g,b` checkerboard,  
`em intensity` emission, `bm size path` bump map, `tx size path` texture

---

## Build Commands

```bash
make              # release: -O3 -flto -march=native + 16 strict warning flags
make debug        # debug: -g3 -O0 -fsanitize=address,undefined,leak,bounds
make re           # clean + release
make rebug        # clean + debug
make help         # list targets
```

Run:
```bash
./rt scenes/figuras_pov.rt
```

---

## Coding Rules

### DO
- Use modern C (C11/C23): `_Atomic`, `_Thread_local`, `static_assert`, `constexpr`, `nullptr`
- Use `<stdbool.h>` for `bool` (already included via MLX42)
- Declare variables at point of use (not old 42 Norm top-of-function)
- Use named variables, not cryptic arrays (e.g., `denom` not `dnrxy[0]`)
- Add `static` to file-internal functions
- Add `const` to read-only parameters
- Include via `-I` flags: `"rt.h"`, `"libvct.h"` (no relative paths)
- Commit small, focused changes with descriptive messages
- Run `make` after every change to verify compilation
- All mandatory part must be PERFECT before adding options

### DON'T
- Don't use old-Norm patterns (separate declaration/assignment, 25-line max)
- Don't use `ft_*` libft functions (being removed, use glibc instead)
- Don't add comments unless truly necessary (explain why, not what)
- Don't commit secrets, large binaries, or build artifacts
- Don't use GPU rasterization pipeline (compute shaders OK)
- Don't create giant commits — one concern per commit
- Don't use `rand()` without `_Thread_local` RNG (not thread-safe)

### Warning Flags Active
16 flags: `-Wshadow -Wunused -Wformat=2 -Wundef -Wstrict-prototypes -Wmissing-prototypes -Winit-self -Wimplicit-fallthrough=3 -Wcast-align -Wswitch-default -Wswitch-enum -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wstack-protector -fstrict-aliasing`

All warnings are `-Werror`. Code must compile with 0 errors, 0 warnings.

---

## Current Priorities (see TODO.md)

1. **Rename project** miniRT → RT (binary, headers, guards)
2. **Remove libft** — replace all `ft_*` with glibc or custom inline
3. **C11+ modernization** — `_Atomic`, `_Thread_local`, `static_assert`, `constexpr`
4. **Remove dead code** — `m_trace` mutex, `trace_flag`, `specular_light()`
5. **Fix remaining bugs** — console NULL checks, material init overlap
6. **Reference scenes** — create 3 mandatory scenes from subject
7. **Documentation** — FORMAT.md, ARCHITECTURE.md

---

## File Map

| Directory | Purpose |
|-----------|---------|
| `inc/` | All headers (rt.h, data.h, render.h, parser.h, console.h, window.h, threads.h, debug.h) |
| `src/main.c` | Entry point |
| `src/parser/` | `.rt` file parser (reads scene, builds object/light/camera lists) |
| `src/render/calcs/` | Core path tracing (find_closest, path_trace, materials, quadratic solver) |
| `src/render/obj/` | Ray-object intersections (sphere, plane, cylinder, cone, caps) |
| `src/render/illumination/` | Lighting (ambient, diffuse, specular, shadows) |
| `src/render/init/` | Initialization (objects, rays, materials, viewport, sides) |
| `src/render/color/` | Color utilities + checkerboard |
| `src/render/texture/` | UV mapping per object type |
| `src/render/bump_map/` | Bump/normal mapping (sphere, plane) |
| `src/render/render/` | Multithreaded render loop + 3 render modes |
| `src/render/free/` | Memory cleanup |
| `src/console/` | In-viewport interactive UI console |
| `src/window/` | MLX42 keyboard/mouse hooks |
| `src/debug/` | Debug print utilities |
| `lib/libvector/` | 3D/2D vector math library |
| `lib/libft/` | Custom C library (**being removed**) |
| `lib/MLX42/` | MLX42 graphics library (window, pixels, events) |
| `scenes/` | `.rt` scene definition files |
| `assets/` | Textures, bump maps, UI images |
