# Architecture — RT Ray Tracer

## Overview

RT is a Monte Carlo path tracer written in C23. It renders 3D scenes defined in `.rt` files using physically-based ray tracing with MLX42 for windowing/pixels.

## Build & Run

```bash
make              # Release: -O3 -flto -march=native -std=gnu23
make debug        # Debug: -g3 -O0 -fsanitize=address,undefined,leak,bounds
./rt scenes/subject_1.rt
```

**Dependencies:** MLX42, libvector, glibc (no libft).

## Directory Map

| Directory | Purpose |
|-----------|---------|
| `inc/` | Headers: rt.h, data.h, render.h, parser.h, console.h, util.h, threads.h, window.h, debug.h |
| `src/main.c` | Entry point |
| `src/util/` | glibc replacements: str_sub, str_trim, str_split, str_join, str_cmp, free_strs, gnl, lst |
| `src/parser/` | `.rt` file parser |
| `src/render/calcs/` | Core path tracing (path_trace, diffuse_ray, materials) |
| `src/render/obj/` | Ray-object intersections (sphere, plane, cylinder, cone, caps, viewport) |
| `src/render/illumination/` | Lighting (ambient, diffuse, specular, shadows) |
| `src/render/init/` | Initialization (rays, image, objects, materials, general) |
| `src/render/color/` | Color utilities + checkerboard |
| `src/render/texture/` | UV mapping |
| `src/render/bump_map/` | Bump/normal mapping |
| `src/render/render/` | Multithreaded render loop + 3 render modes |
| `src/render/free/` | Memory cleanup |
| `src/console/` | Interactive UI console (click-to-edit objects/lights/camera) |
| `src/window/` | MLX42 keyboard/mouse hooks |
| `src/debug/` | Debug print utilities |
| `lib/libvector/` | 3D/2D vector math library |
| `lib/MLX42/` | Graphics library (window + pixel buffer) |

## Data Flow

```
main.c
  ├── init_data()           → allocates t_data
  ├── validate_args_and_open() → opens .rt file
  ├── parse()               → reads .rt, builds object/light/camera lists
  ├── init_mlx()            → creates window + image buffer
  ├── init_all()            → precomputes per-object ray data
  └── mlx_loop()            → event loop
       ├── render_fast()    → low-res preview + console UI
       ├── render_one()     → full path trace (1 sample)
       └── update_render()  → progressive accumulation (weighted average)
```

## Render Pipeline

```
render(data, mode)
  ├── init_viewport(cam, w, h)  → camera frame, viewport bounds
  ├── init_rays(data, cam, vp)  → ray grid (1920×1080) with jitter
  ├── init_image_(data)         → image buffer (uint32_t **)
  ├── render_with_threads()     → 6 pthread workers
  │   └── process_rows() → trace_ray(rays[y][x], data) per pixel
  └── free_render(vp, rays)
```

## Per-Pixel: trace_ray() → path_trace()

```
trace_ray(ray, data)
  ├── find_closest()         → linear scan all objects for nearest hit
  └── path_trace(ray, data, MAX_DEPTH=5)  → recursive bounce
       ├── pt_checks()       → base color (albedo or checkerboard)
       ├── compute_direct_light() → ambient + diffuse + specular + emission
       │   ├── iter_lights()  → per point light: shadow ray + Phong
       │   └── compute_emissive_light() → EM objects as area lights
       ├── apply_self_emission() → EM material self-glow
       └── path_trace_type()  → material-specific bounce
           ├── MAT_DEFAULT → diffuse_ray()  → random hemisphere (cosine-weighted)
           ├── MT         → metallic_ray() → perturbed reflection
           ├── MR         → mirror_ray()   → perfect reflection
           └── GL         → glass_ray()    → Fresnel: refract + reflect
```

## Render Modes

| Mode | Function | Description |
|------|----------|-------------|
| FAST | `render_fast()` | Low-res preview, 1 ray per 10×10 block, console UI overlay |
| ONE | `render_one()` | Full path trace, 1 sample/pixel with jitter, no accumulation |
| UPDATE | `update_render()` | Progressive: weighted average over time, sample_count tracking |

## Object Types

```
SP=0 (sphere), PL=1 (plane), CY=2 (cylinder), CAP=3 (end cap),
CO=6 (cone), CU=7 (cube), SIDE=8 (cube face)
```

## Material Types

```
MT=0 (metallic), GL=1 (glass), MR=2 (mirror), CB=3 (checkerboard),
EM=4 (emission), BM=5 (bump map), TX=6 (texture), MAT_DEFAULT=-1 (diffuse)
```

## Thread Safety

- `_Thread_local` xorshift32 RNG per worker thread (no shared `rand()` state)
- `_Atomic bool god` for render start/stop signaling (lock-free, no mutex)
- 6 threads (`NUM_THREADS`), row-interleaved work distribution

## Key Constants

```
MAX_DEPTH=5        path tracing bounce limit
EPSILON=1e-3       ray offset to avoid self-intersection
NUM_THREADS=6      worker thread count
W_WH=1920, W_HG=1080  default viewport
```
