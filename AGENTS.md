# AGENTS.md — AI Agent Instructions for RT (Ray Tracer)

## Project Overview

**RT** is a Monte Carlo path tracer written in C (C23 ready), part of the 42 school curriculum.  
It renders 3D scenes defined in `.rt` scene files using physically-based ray tracing.

**Subject:** `en.subject.md` (version 4.1)  
**Language:** C (modern, C11/C23 features encouraged)  
**Binary:** `rt`  
**Build:** `make` (release `-O3 -flto`), `make debug` (sanitizers)  
**Dependencies:** MLX42 (window/pixels), libvector (math), glibc only (no libft)

---

## Project State

| Task | Status |
|------|--------|
| libft removed, replaced with glibc + `src/util/` | ✅ Done |
| Project renamed miniRT → RT (binary, headers, guards) | ✅ Done |
| Build system: `-O3 -flto -march=native` + 16 strict warnings | ✅ Done |
| C11+ modernization (`_Atomic`, `_Thread_local`, `constexpr`) | 🔴 Pending |
| Console bugs (NULL checks, dead code) | 🟡 Pending |
| Cleanup: `m_trace`, `trace_flag`, `specular_light()` dead code | 🟡 Pending |
| Reference scenes (3 mandatory) | 🟡 Pending |

---

## Architecture Flow

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

## How Objects Are Detected (Parsing Flow)

### `type_obj()` — Prefix-Based Type Detection

Located in `src/parser/utils/parse_utils.c`. The function uses `str_cmp()` (NOT standard `strcmp`) to match the 2-char prefix of each `.rt` file line against known types:

```c
static const char *bts[] = {"sp", "pl", "cy", "A", "C", "L", "co", "cu", NULL};
```

`str_cmp("sp", "sp -45,3,0 15 255,0,0 mt")` returns `-1` because `"sp"` IS a prefix. `!(-1)` = `0`, loop stops → match found → returns `0` (SP).

`str_cmp("sp", "A 0.2 230,230,230")` returns `0` (neither is prefix). `!(0)` = `1`, loop continues to next type.

This is why `str_cmp` must NOT be standard `strcmp` — standard would return char difference `!nonzero=0` and stop on the first type.

### `parse()` — Main Parse Loop

Located in `src/parser/parser/parser.c`. Reads the `.rt` file line by line with `get_next_line()`:

```
parse(data, fd)
  ├── while (str = get_next_line(fd))
  │   ├── skip comments (#)
  │   ├── type = type_obj(str)
  │   │   ├── type <= 2 || CO || CU  →  create_obj(data, str, type)
  │   │   │   (sp=0, pl=1, cy=2, co=6, cu=7 — geometric objects)
  │   │   └── type > 7  →  parse error
  │   ├── create_alight(data, str, type)   — type=3(A), has guard
  │   ├── create_cam(data, str, type)      — type=4(C), has guard
  │   └── create_slight(data, str, type)   — type=5(L), has guard
  └── check_params(data)  →  final validation
```

Each `create_*` function has a guard that checks `type` and returns immediately if wrong. So all three are called for every line, but only the matching one activates.

### `create_obj()` — Geometric Object Creation

Located in `src/parser/obj/parse_obj.c`. Parses the line according to object type:

```
create_obj(data, str, type)
  ├── new_obj(data)            →  calloc + set data, type
  ├── doubles_parse()          →  extract x,y,z position from str[2:...]
  ├── doubles_parse(tmp, 0, 1) →  extract axis (pl/cy/co/cu) or skip (sp)
  ├── create_obj_normi()       →  parse size, height, cube dimensions
  ├── colors_parse()           →  extract r,g,b
  └── extra_functionalities()  →  parse material suffixes (mt, gl, mr, cb, em, bm, tx)
```

### ACL Objects (Ambient, Camera, Light)

- **create_alight()** (`parse_acl.c`): Parses `A br r,g,b`
- **create_cam()** (`parse_acl.c`): Parses `C x,y,z nx,ny,nz fov [focus_dist] [aperture]`
- **create_slight()** (`parse_acl.c`): Parses `L x,y,z br r,g,b`

---

## How Objects Are Rendered (Full Path Trace Flow)

### Entry Points

Three render modes call into the same core renderer differently:

```
render_one()    → render(data, mode=1) → init_rays()     (1 ray/pixel)
render_fast()   → console_render(data)  → init_raysc()   (1 ray per 10x10 block)
update_render() → render(data, mode=0) → init_rays()     (1 ray/pixel, accumulated)
```

### Core Render Loop

```
render(data, mode)
  ├── init_viewport(cam, width, height)    → compute camera frame, viewport bounds
  ├── init_rays(data, cam, vp)             → allocate ray grid (1920×1080)
  │   ├── init_ray_row() → for each pixel, generate UV coordinates + jitter
  │   └── init_single_ray() → ray.origin = cam.pos, ray.direction from UV
  ├── init_image_(data)                     → allocate image buffer (uint32_t **)
  ├── render_with_threads(data, rays, img)  → NUM_THREADS=6 pthread workers
  │   └── process_rows() → per-pixel: trace_ray(rays[y][x], data)
  └── free_render(vp, rays)
```

### Per-Pixel: `trace_ray()` → `path_trace()`

```
trace_ray(ray, data)
  ├── find_closest(data, &ray, data->obj, &t_min)  → linear scan all objects
  │   ├── for each obj: hit_sp | hit_pl | hit_cy | hit_cap | hit_cone
  │   └── keep closest with t > 0 && t < t_min
  ├── path_trace(&ray, data, MAX_DEPTH=5)           → recursive bounce
  │   ├── find_closest() again for the current ray
  │   ├── pt_checks() → compute base color (albedo or checkerboard)
  │   ├── compute_direct_light() → ambient + diffuse + specular + emission
  │   │   ├── iter_lights() → for each point light
  │   │   │   ├── data_shadow() → shoot shadow ray, check occlusion
  │   │   │   ├── difuse_light() → Lambertian diffuse term
  │   │   │   └── specular highlight → Phong via reflect + pow
  │   │   └── compute_emissive_light() → for EM objects acting as area lights
  │   ├── apply_self_emission() → if material is EM, add self-emission glow
  │   └── path_trace_type() → material-specific bounce (recursive)
  │       ├── m_type == -1  → diffuse_ray()  → random hemisphere bounce
  │       ├── m_type == MT  → metallic_ray() → perturbed reflection
  │       ├── m_type == MR  → mirror_ray()   → perfect reflection
  │       └── m_type == GL  → glass_ray()    → Fresnel: refract + reflect
  └── texture/blending applied to final color
      ├── material.texture → texture_color() lookup
      └── material.bm_texture && material.texture → texture_weight() blend
```

### Object Intersection Detail

| Object | Function | File | Algorithm |
|--------|----------|------|-----------|
| Sphere | `hit_sp()` | `obj/intersections.c` | Quadratic (a,b,c from ray-sphere equation) |
| Plane | `hit_pl()` | `obj/intersections.c` | Dot product: `t = (P0 - O)·N / D·N` |
| Cylinder | `hit_cy()` | `obj/cylinder.c` | Quadratic in perpendicular space + height clamp |
| Cone | `hit_cone()` | `obj/cone.c` | Quadratic with half-angle parameter + height clamp |
| Cap | `hit_cap()` | `obj/caps.c` | Plane intersection + radius check |
| Cube | decomposed to `SIDE` | `init/init_side.c` | Cube → 6 rectangular planes with local bounds |

### Material Bounce Detail

```
glass_ray(ray, closest, data, depth)
  ├── fresnel() → compute reflection/refraction ratio via Fresnel equations
  ├── reflected_ray → new origin at hit point + EPSILON*normal, direction = reflect(dir, normal)
  ├── refracted_ray → new origin at hit point - EPSILON*normal, direction = refract(dir, normal)
  └── return blend(reflected_color * kr, refracted_color * (1 - kr))

mirror_ray(ray, closest, data, depth)
  ├── new_ray → origin at hit+EPSILON, direction = reflect(dir, normal)
  └── return path_trace(&new_ray, data, depth-1) * reflectivity

metallic_ray(ray, closest, data, depth)
  ├── new_ray → origin at hit+EPSILON, direction = reflect(dir, normal)
  ├── if roughness > 0 → perturb_vector() randomizes direction in hemisphere
  └── return path_trace(&new_ray, data, depth-1) * reflectivity

diffuse_ray(ray, closest, data, depth)
  ├── new_ray → origin at hit+EPSILON, direction = random_in_hemisphere(normal)
  └── return path_trace(&new_ray, data, depth-1) * reflectivity
```

### Lighting Detail

```
data_shadow(data, shadow_ray, max_dist, self)
  ├── Shoot ray from hit point toward light
  ├── Check ALL objects for intersection between hit and light
  ├── Skip self and same-parent objects (avoids self-shadowing)
  └── Returns true if blocked (in shadow)

iter_lights(data, obj, ray, direct)
  ├── For each point light:
  │   ├── Compute light direction and distance
  │   ├── data_shadow() → check occlusion
  │   ├── difuse_light() → Lambert: color += obj.rgb * light.rgb_intensity * N·L
  │   └── Specular → Phong: color += light.rgb * (reflect(L)·V)^shininess * specularity
  └── Returns direct lighting color

apply_al(obj_color, ambient_light)
  └── result = obj_color * (ambient.rgb/255.0) * ambient.brightness
```

---

## Render Modes

### `render_one()` (mode = ONE, 1)
- Full path trace with **1 sample per pixel**
- Uses `init_rays()` — one ray per pixel with random jitter for antialiasing
- **No accumulation** — each frame is independent
- Highest visual quality (full path trace, MAX_DEPTH=5)
- Maps to `render_sel = render_one`, press `SPACE` to toggle here

### `render_fast()` (mode = FAST, 0)
- **Console mode**: renders low-res preview + shows interactive UI
- Uses `init_raysc()` — one ray per **10×10 pixel block**
- Uses `trace_fast()` instead of `trace_ray()`:
  - NO path tracing — just finds closest object and returns its ambient color
  - Fills the 10×10 block with the same color
- **80× faster** than full path trace
- Console overlaid on top: clickable buttons to modify objects/lights/camera in real time
- Camera movement enabled (arrows, Ctrl+arrows)
- Maps to `render_sel = render_fast`, press `C` or click to enter

### `update_render()` (mode = UPDATE, 2)
- **Progressive accumulation** — averages samples over time
- Each frame: render 1 sample, average with previous accumulation via `average_samples()`
- Uses `init_rays()` — one ray per pixel with random jitter
- First frame: stores result as `img_last`
- Subsequent frames: render new sample, average with `img_last` via `average()`
- Quality improves over time (less noise with more samples)
- Maps to `render_sel = update_render`, press `SPACE` to toggle here

---

## Key Data Structures

| Header | Contents |
|--------|----------|
| `inc/rt.h` | Master header — includes everything, color macros, type constants |
| `inc/data.h` | `t_data`, `t_obj`, `t_cam`, `t_material`, `t_calcs`, `t_alight`, `t_slight`, `t_console`, `t_list` |
| `inc/render.h` | Rendering data: `t_vp`, `t_ray`, `t_direct`, `t_quadratic`, render function prototypes |
| `inc/parser.h` | Parser: `parse()`, `create_obj()`, `create_cam()`, `create_slight()`, conversion functions |
| `inc/console.h` | Console UI: button enums, label/click/icon function prototypes |
| `inc/util.h` | Utility functions: `str_sub`, `str_trim`, `str_split`, `str_join`, `str_cmp`, `free_strs`, `get_next_line`, `lst_*` |
| `inc/threads.h` | `t_thread_data`, `render_with_threads` |
| `inc/window.h` | Keyboard/mouse hooks, `fill_image`, `swap_mgod` |
| `inc/debug.h` | `print_t_rgb`, `print_t_v3`, `current_timestamp` |

### Object Types (defined in `inc/rt.h`)
```
SP=0   sphere
PL=1   plane
CY=2   cylinder
CAP=3  end cap (cylinder/cone cap)
CO=6   cone
CU=7   cube
SIDE=8 cube face (decomposed from CU)
```

### Material Types (defined in `inc/rt.h`)
```
MT=0   metallic (rough reflection)
GL=1   glass (Fresnel: refraction + reflection)
MR=2   mirror (perfect reflection)
CB=3   checkerboard
EM=4   emission (light source)
BM=5   bump map
TX=6   texture
```

### Ray Tracing Pipeline
1. **Ray generation** (`src/render/init/init_rays.c`): creates ray grid from camera/viewport
2. **Intersection** (`src/render/obj/*.c`): `find_closest()` in `calcs.c` tests all objects, returns closest hit
3. **Lighting** (`src/render/illumination/illumination.c`): ambient + diffuse + specular per light source
4. **Materials** (`src/render/calcs/materials.c`): glass (Fresnel), mirror, metallic, diffuse
5. **Path tracing** (`src/render/calcs/calcs.c`): recursive bounces up to `MAX_DEPTH=5`

---

## Scene File Format (`.rt`)

```
A    br r,g,b                         # ambient light
C    x,y,z  nx,ny,nz  fov [fd] [ap]   # camera (fd=focus dist, ap=aperture)
L    x,y,z  br  r,g,b                 # point light
sp   x,y,z  diameter  r,g,b  [mat...] # sphere
pl   x,y,z  nx,ny,nz  r,g,b  [mat...] # plane
cy   x,y,z  nx,ny,nz  d  h  r,g,b  [mat...] # cylinder (d=diameter, h=height)
co   x,y,z  nx,ny,nz  d  h  r,g,b  [mat...] # cone
cu   x,y,z  nx,ny,nz  w  h  d  r,g,b  [mat...] # cube
#    comment line (ignored)
```

Material suffixes (after color): `mt` metallic, `gl` glass, `mr` mirror, `cb scale r,g,b` checkerboard, `em intensity` emission, `bm size path` bump map, `tx size path` texture

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

**Controls in runtime:**
- `C` — open/close interactive console (also click mouse)
- `SPACE` — toggle between single render and progressive accumulation
- `ESC` — quit
- `Arrows` — move camera (only in FAST/console mode)
- `Ctrl+↑/↓` — zoom FOV

---

## File Map

| Directory | Purpose |
|-----------|---------|
| `inc/` | Headers: `rt.h`, `data.h`, `render.h`, `parser.h`, `console.h`, `util.h`, `threads.h`, `window.h`, `debug.h` |
| `src/main.c` | Entry point |
| `src/util/` | **glibc replacements:** `str_sub`, `str_trim`, `str_split`, `str_join`, `str_cmp`, `free_strs`, `gnl` (get_next_line), `lst` (linked list) |
| `src/parser/` | `.rt` file parser |
| `src/render/calcs/` | Core path tracing |
| `src/render/obj/` | Ray-object intersections |
| `src/render/illumination/` | Lighting |
| `src/render/init/` | Initialization |
| `src/render/color/` | Color utilities + checkerboard |
| `src/render/texture/` | UV mapping |
| `src/render/bump_map/` | Bump/normal mapping |
| `src/render/render/` | Multithreaded render loop + 3 render modes |
| `src/render/free/` | Memory cleanup |
| `src/console/` | Interactive UI console |
| `src/window/` | MLX42 keyboard/mouse hooks |
| `src/debug/` | Debug print utilities |
| `lib/libvector/` | 3D/2D vector math library (external) |
| `lib/MLX42/` | Graphics library (external, built via cmake) |
| `scenes/` | `.rt` scene definition files |
| `assets/` | Textures, bump maps, UI images |

---

## Coding Rules

### DO
- Use modern C (C11/C23): `_Atomic`, `_Thread_local`, `static_assert`, `constexpr`, `nullptr`
- Declare variables at point of use (not old 42 Norm top-of-function)
- Use named variables, not cryptic arrays
- Add `static` to file-internal functions
- Add `const` to read-only parameters
- All includes use `-I` flags: `"rt.h"`, `"libvct.h"`, `"util.h"` (no relative paths)
- Commit small, focused changes with descriptive messages
- Run `make` after every change — must compile with 0 errors, 0 warnings

### DON'T
- Don't use `ft_*` functions (libft has been REMOVED)
- Don't use `rand()` without `_Thread_local` RNG (not thread-safe)
- Don't use GPU rasterization pipeline (compute shaders OK per subject)
- Don't add comments unless explaining why, not what
- Don't commit secrets, binaries, or build artifacts
- Don't create giant commits — one concern per commit

### ⚠️ Critical Gotchas

**`str_cmp()` is NOT a standard `strcmp`!**  
The original `ft_strcmp` had custom behavior: it returns **0** when neither string is a prefix of the other, and **-1** when one IS a prefix. This is used by `type_obj()` and `type_extra_func()` to match scene file type prefixes (e.g., `"A"` matches `"A\t\t0.2..."`).  
File: `src/util/str_cmp.c` — DO NOT replace with standard `strcmp`.

**`IS_SPACE(c)` / `IS_DIGIT(c)` macros, NOT raw `isspace`/`isdigit`**  
Defined in `inc/rt.h`. Always cast to `(unsigned char)` for safety with glibc ctype functions. Scene files contain UTF-8 (Spanish comments like "Cámara") — signed char promotion would be UB without the cast.

**`get_next_line()` implementation is fragile**  
Located in `src/util/gnl.c`. Reads 1 byte at a time from the fd. The static buffer holds all remaining data between calls. The `gnl_while_reading` function has a known gotcha: on EOF (byte_count==0), it must free the unused buffer and return line, NOT call `gnl_free_join` which would free `line` too.

**`parse_atof()` / `parse_atoi()` have custom validation**  
Located in `src/parser/utils/parse_conversions.c`. These were `ft_atof`/`ft_atoi_parse` — renamed to remove `ft_` prefix. They have built-in error handling via `exit(er(...))` on invalid input.

---

## Warning Flags (all `-Werror`)

```
-Wall -Wextra -Wshadow -Wunused -Wformat=2 -Wundef
-Wstrict-prototypes -Wmissing-prototypes -Winit-self
-Wimplicit-fallthrough=3 -Wcast-align -Wswitch-default
-Wswitch-enum -Wduplicated-branches -Wduplicated-cond
-Wlogical-op -Wstack-protector -fstrict-aliasing
```

**0 errors, 0 warnings required.**

---

## Current Priorities (see `TODO.md` for full list)

1. **C11+ modernization** — `_Atomic bool` for `data->god` (replaces `pthread_mutex`), `_Thread_local` per-thread RNG (replaces `rand()`), `static_assert`, `constexpr`
2. **Remove dead code** — `m_trace` mutex + `trace_flag` (never read), `specular_light()` (never called)
3. **Fix console bugs** — NULL checks after calloc, NULL deref guards on `last_item`, dead code cleanup
4. **Material system refactor** — merge `init_materials()` (parser) + `init_materials_render()` (render) into single function
5. **Reference scenes** — create 3 mandatory scenes matching subject figures
6. **Documentation** — `FORMAT.md`, `ARCHITECTURE.md`

---

## Useful Search Patterns

When debugging or adding features, these grep patterns help navigate the codebase:

```bash
# Find where a function is defined or called
grep -rn 'function_name' src/ inc/

# Find all object intersection handlers
grep -rn 'hit_' src/render/obj/

# Find all material type checks
grep -rn 'material\.m_type' src/

# Find where a specific scene parameter is parsed
grep -rn 'create_obj\|create_cam\|create_alight\|create_slight' src/parser/

# Check for memory leaks (requires debug build)
make debug && ./rt scenes/figuras_pov.rt
```
