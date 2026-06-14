# TODO

---

## 🔴 BLOCKER — Project Identity (RT, not miniRT)

- [ ] Rename executable: `miniRT` → `rt` (Chapter IV: *"The executable file must be named `rt`"*)
- [ ] Rename directories/files: `miniRT.h` → `rt.h`, `mini_rt/` → `rt/`
- [ ] Update Makefile `NAME := rt`
- [ ] Update all `#include "miniRT.h"` → `#include "rt.h"`
- [ ] Update header guards: `MINIRT_H` → `RT_H`
- [ ] Scene file extension: keep `.rt` (already correct)
- [ ] Verify subject v4.1 requirements against current codebase

---

## 🔴 BLOCKER — Eliminate libft (use glibc)

Chapter IV: *"you are allowed to use the entire libc"*

> Map: `ft_xxx` → glibc or custom inline

| Priority | ft_function | Uses | Replacement |
|----------|------------|------|-------------|
| 1 | `ft_isspace` (31) | 31 | `isspace()` from `<ctype.h>` |
| 1 | `ft_isdigit` (19) | 19 | `isdigit()` from `<ctype.h>` |
| 1 | `ft_strlen` (12) | 12 | `strlen()` from `<string.h>` |
| 1 | `ft_memcpy` (1) | 1 | `memcpy()` from `<string.h>` |
| 1 | `ft_strcmp` (2) | 2 | `strcmp()` from `<string.h>` |
| 2 | `ft_printf` (16) | 16 | `printf`/`dprintf` from `<stdio.h>` (or keep if complex formatting) |
| 2 | `ft_substr` (12) | 12 | Custom: `strndup(src + start, len)` or inline |
| 2 | `ft_strtrim` (4) | 4 | Custom: trim whitespace inline (~15 lines) |
| 2 | `ft_split` (1) | 1 | Custom: write simple split or use `strtok_r` |
| 2 | `ft_atof` (18) | 18 | `strtod()` from `<stdlib.h>` + custom validation |
| 2 | `ft_atoi_parse` (11) | 11 | `strtol()` from `<stdlib.h>` + bounds checking |
| 2 | `ft_atof_normi` (3) | 3 | Merged into `strtod` usage |
| 3 | `ft_lstnew` (24) | 24 | `calloc(1, sizeof(t_btn))` + manual append |
| 3 | `ft_lstadd_front` (21) | 21 | Manual linked-list insert |
| 3 | `ft_lstadd_back` (3) | 3 | Manual linked-list append |
| 3 | `ft_lstclear` (3) | 3 | Manual while loop free |
| 3 | `ft_objremove` (3) | 3 | **KEEP** — custom linked-list remove, not from libft |
| 3 | `ft_free_willy` (4) | 4 | **RENAME** — just a `free_strs()` function |

### Strategy
1. Replace simple 1:1 wrappers (`ft_isspace`→`isspace`, etc.) — bulk sed
2. Rewrite `ft_printf` call sites to use `printf`/`dprintf`/`fprintf`
3. Replace `ft_substr`/`ft_strtrim`/`ft_split` with small inline alternatives
4. Replace `ft_atof`/`ft_atoi_parse` with `strtod`/`strtol`
5. Replace `ft_lst*` linked-list functions with manual operations (only used by console, ~8 functions)
6. Remove `lib/libft/` from build
7. Remove `-I./lib/libft/inc` from Makefile
8. Delete `lib/libft/` directory

---

## 🟡 HIGH — C11+/C23 Modernization

Chapter V: *"use the latest version of the language and follow up-to-date good practice"*

### Thread Safety (replace `rand()` + mutexes)

- [ ] **`_Thread_local` per-thread RNG:** Replace `rand()` (not thread-safe) with `_Thread_local uint64_t rng_state` + xorshift64
  - Files: `calcs_utils.c` (`random_in_hemisphere`), `init_rays.c` (`random_in_unit_disk`, `init_ray_row`)
  - Removes the last remaining thread-unsafety
- [ ] **`_Atomic bool` for `data->god`:** Replace `pthread_mutex_t *m_god` + `pthread_mutex_lock/unlock` 
  - Files: `render/render.c` (`process_rows`), `window/mlx.c` (`resise_w`), `window/mlx_utils.c` (`swap_mgod`)
  - Much faster: atomic operations are lock-free
  - Remove `pthread_mutex_init(&data->m_god)` from `init_general.c`
- [ ] **Remove `m_trace` mutex entirely:** Already removed the lock/unlock, now delete the init/free
  - `trace_flag` variable is never read — dead code, remove it
  - Files: `init_general.c`, `mlx_utils.c`, `data.h`, `parse_utils.c`, `main.c`

### Type Safety & Compile-Time Checks

- [ ] **`static_assert`** — Verify struct sizes and alignment at compile time
  - `static_assert(sizeof(t_obj) <= 256, "t_obj too large")`
  - `static_assert(sizeof(t_calcs) <= 256, "t_calcs too large")`
- [ ] **`constexpr` (C23)** — Replace `#define` constants where possible
  - `constexpr double EPSILON = 1e-3;`
  - `constexpr int MAX_DEPTH = 5;`
- [ ] **`enum` with explicit type (C23)** — Save memory on enums
  - `enum item_type : uint8_t { CAM = 0, ALIGHT, SLIGHT, OBJ };`
  - `enum render_mode : uint8_t { FAST = 0, ONE, UPDATE };`
- [ ] **`nullptr` (C23)** — Replace `NULL` macro
- [ ] **`typeof` (C23)** — Simplify `swap()` macro: `#define swap(a,b) do { typeof(a) t=a; a=b; b=t; } while(0)`

### Code Modernization

- [ ] **Designated initializers** — Already partially used, extend to struct initialization
- [ ] **Compound literals** — Already used for `t_v3`, extend where missing
- [ ] **`restrict` qualifier** — Add to hot-path pointers in intersection functions
  - `hit_sp(t_ray *restrict ray, t_obj *restrict sphere, double *restrict t)`
- [ ] **`const` correctness** — Add `const` to read-only parameters throughout
- [ ] **Replace `bool` from libft** — Use `<stdbool.h>` standard `bool` (already included via MLX42 headers)

---

## 🟡 HIGH — Mandatory Part Requirements (verify)

> All mandatory items must be "PERFECT" before options are evaluated

- [ ] **4 geometric objects:** ✅ sphere, plane, cylinder, cone
- [ ] **Translation + rotation transformations:** ⚠️ Only axis-based rotation. Need proper 4x4 matrix transforms
  - File: new `src/math/transform.c` with translate/rotate/scale matrices
  - Apply to all `t_obj` before rendering
  - Scene file: add `tr x,y,z` and `rt axis_deg,x,y,z` syntax
- [ ] **Camera repositionable:** ✅ Via console and keyboard
- [ ] **Redraw without recalculating everything:** ✅ `update_render` accumulates
- [ ] **Light management:** ✅ Multiple spots, shadows, brightness, specular/shine
- [ ] **3 reference scenes:** Need to create scenes matching the subject figures
  - Scene 1: 4 basic objects, 2 spots, shadow + shine
  - Scene 2: Same from different viewpoint
  - Scene 3: Shadow mixing

---

## 🟢 MEDIUM — Remaining Code Issues

### Console Code
- [ ] No NULL check after `calloc` in ~20 button creation functions
- [ ] Potential NULL deref of `last_item` in label/icon functions
- [ ] `slight->next` deref without NULL guard in `console_click_obj_utils.c:37`
- [ ] Dead code: always-true `if (i < 4)` in `console_icons.c:113`
- [ ] Dead assignments: `obj = last_item` before type guard in 4 files

### Material System
- [ ] Material type `-1` means "default" — magic number, use enum `MAT_DEFAULT`
- [ ] `init_materials()` (parser) + `init_materials_render()` (render) have overlapping logic
  → refactor into single `material_init(t_obj *obj)` called once
- [ ] `emision` (parser field) vs `self_emision` (computed `emision * 2.5`) — document the 2.5 factor

### Lighting
- [ ] `apply_al()` normalizes RGB to 0-1 and back — lossy for dark colors. Fix: use float colors internally
- [ ] `difuse_light()` mixes `obj->rgb` with light `rgb_inty` via integer division → banding
- [ ] `specular_light()` (unused!) — referenced in header but never called; remove or integrate
- [ ] `compute_emissive_light()` passes `emitter` as `self` to `data_shadow` but `parent` logic is broken
  (parent is an incrementing counter, not inherited by caps/sides)

### Geometry
- [ ] `check_obj` cone hack: `obj->size += 35` — magic number, document as "cone angle correction"
- [ ] `half_angle` computed AFTER `size += 35` — coupling hidden between `check_obj` and `init_obj_normi`
- [ ] `cone_calc` formula uses `calcs.k = cos_half²` — verify against standard cone equation
- [ ] `hit_cy` uses `EPSILON` for height bounds but `EPSILON` is `1e-3` — too large for thin cylinders?

### Memory & Safety
- [ ] `last_exit()` calls `mlx_terminate()` before freeing objects — potential use-after-free if MLX callbacks fire
- [ ] `er()` function calls `exit()` but returns `int` — change return type to `_Noreturn void`
- [ ] `t_data->args` (global args from parser) — stored in `data->args`, freed in `er()`. Move to local var
- [ ] Add Valgrind/ASan run to CI or manual testing checklist
- [ ] `init_raysc()` (console render) — same signature as `init_rays()` but different behavior, document

---

## 🔵 LOW — Polish & Consistency

- [ ] Rename `t_slight` → `t_light` (it's not just "spot", it's point light)
- [ ] Rename `t_alight` → `t_ambient`
- [ ] Rename `t_rgb` → `t_color` or keep `t_rgb`
- [ ] Split `data.h` (382 lines) → `types.h` + `scene.h` + `material.h`
- [ ] Split `render.h` (269 lines) → `ray.h` + `viewport.h` + `intersect.h` + `lighting.h`
- [ ] Document scene `.rt` file format in `doc/FORMAT.md`
- [ ] Add `doc/ARCHITECTURE.md` explaining the ray tracing pipeline
- [ ] Console enum `t_btn_name` has intentional value collisions — document or separate
- [ ] Console `t_img_btn.row1[200]` etc. — replace fixed-size buffers with dynamic strings
- [ ] Error messages in Spanish → English (or keep bilingual)
- [ ] `debug_info.c` functions unused in release — wrap in `#ifdef DEBUG`
- [ ] `specular_light()` declared in header but never called — dead API, remove

---

## 💡 IDEAS — Future Enhancements (Options/Bonuses)

- [ ] GPU acceleration via OpenCL compute shaders (subject allows)
- [ ] Full 4x4 matrix transform system (translate + rotate + scale)
- [ ] Bounding Volume Hierarchy (BVH) for faster intersection tests
- [ ] Adaptive supersampling (more samples at edges)
- [ ] Depth of field (already partially implemented with `aperture`/`focus_dist`)
- [ ] Motion blur
- [ ] Environment maps / HDR skyboxes
- [ ] `.obj` mesh loading and ray-triangle intersection
- [ ] Parallel/distributed rendering (network)
- [ ] Video output (image sequence → ffmpeg)
- [ ] raylib backend (drop MLX42 dependency)
- [ ] IMGUI-style in-viewport debug overlay (replace custom console)

---

## 📋 Work Sequence

1. **Rename project** `miniRT` → `rt` (binary, headers, guards, includes)
2. **Eliminate libft** (replace all `ft_*` → glibc/custom)
3. **C11+ modernization** (`_Thread_local`, `_Atomic`, `static_assert`, `constexpr`)
4. **Remove dead code** (`m_trace`, `trace_flag`, `specular_light`)
5. **Fix remaining console bugs** (NULL checks, dead code)
6. **Refactor material system** (merge init_materials + init_materials_render)
7. **Create reference scenes** (3 mandatory scenes from subject)
8. **Documentation** (FORMAT.md, ARCHITECTURE.md)
9. **Polish** (rename types, split headers, cleanup)

