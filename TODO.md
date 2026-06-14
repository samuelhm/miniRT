# TODO

---

## 🟡 HIGH — C11+/C23 Modernization

Chapter V: *"use the latest version of the language and follow up-to-date good practice"*

### Thread Safety (replace `rand()` + mutexes)

- [x] **`_Thread_local` per-thread RNG:** Replace `rand()` (not thread-safe) with `_Thread_local uint32_t rng_state` + xorshift32
  - Files: `calcs_utils.c` (`random_in_hemisphere`), `init_rays.c` (rand() kept in main thread)
  - Commit: `015afaf`
- [x] **`_Atomic bool` for `data->god`:** Replaced `pthread_mutex_t *m_god` + `pthread_mutex_lock/unlock` with `_Atomic bool god`
  - Files: `render/render.c` (`process_rows`), `window/mlx.c` (`resise_w`, keyhooks), `window/mlx_utils.c` (removed `swap_mgod`), `init_general.c`, `main.c`, `parse_utils.c`
  - Removed `pthread_mutex_t *m_god` entirely: no more alloc/free/init for it
  - Added `#include <stdatomic.h>` to `data.h`
- [x] **Remove `m_trace`/`trace_flag` dead code:** Deleted mutex alloc/free/lock/unlock, trace_flag field removed from `t_data`, `swap_flag_mlx()` removed
  - Files: `init_general.c`, `mlx_utils.c`, `data.h`, `window.h`, `parse_utils.c`, `main.c`, `mlx.c`
- [x] **Remove `specular_light()` dead code:** Never called, removed `specular.c`, prototype from `render.h`, entry from `Makefile`

### Type Safety & Compile-Time Checks

- [x] **`static_assert`** — Verify struct sizes and alignment at compile time
  - `static_assert(sizeof(t_obj) <= 1024, "t_obj too large")`
  - `static_assert(sizeof(t_calcs) <= 512, "t_calcs too large")`
- [x] **`constexpr` (C23)** — Replaced `#define` constants: EPSILON, MAX_DEPTH, NUM_THREADS, W_WH, W_HG
- [x] **`enum` with explicit type (C23)** — `item_type : uint8_t`, `render_mode : uint8_t`
- [x] **`nullptr` (C23)** — Available now with `-std=gnu23` in Makefile
- [x] **`typeof` (C23)** — Replaced `swap()` function with typeof-based macro

- [x] **`restrict` qualifier** — Added to hot-path pointers in intersection functions (hit_sp, hit_pl, hit_cy, hit_cap, hit_cone)
- [x] **`const` correctness** — `const` already widely used; `_Atomic bool` for god

- [x] **`er()` returns `int` but always `exit()`s** — Changed to `_Noreturn void`, removed wrapper `exit(er(...))` calls, added `exit(1)` inside
- [x] **`debug_info.c` functions unused in release** — Wrapped print functions in `#ifdef DEBUG`, kept `current_timestamp()` public
- [x] **`typeof` swap macro** — Replaced `void swap(double*,double*)` function with `#define swap(a,b)` using `typeof(*(a))`
- [x] **Designated initializers** — Already partially used
- [x] **Compound literals** — Already used for `t_v3`

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
- [x] **3 reference scenes:** Created scenes matching the subject figures
  - `scenes/subject_1.rt`: 4 basic objects (sp, pl, cy, co), 2 spot lights, shadow + specular shine
  - `scenes/subject_2.rt`: Same scene from different viewpoint (camera rotated 90°)
  - `scenes/subject_3.rt`: Shadow mixing — spheres casting shadows on each other

---

## 🟢 MEDIUM — Remaining Code Issues

### Console Code
- [x] No NULL check after `calloc` in ~20 button creation functions — added `if (!btn) return;`
- [x] Potential NULL deref of `last_item` in label/icon functions — moved assignments inside type-guarded branches
- [x] `slight->next` deref without NULL guard in `console_click_obj_utils.c:37` — added `slight &&` to condition, moved inside `left` branch
- [x] Dead code: always-true `if (i < 4)` in `console_icons.c:113` — removed
- [x] Dead assignments: `obj = last_item` before type guard in 4 files — moved inside OBJ branches

### Material System
- [x] Material type `-1` means "default" — magic number, use enum `MAT_DEFAULT`
- [x] `init_materials()` (parser) + `init_materials_render()` (render) have overlapping logic → merged: per-type init moved to parser, `init_materials_render()` removed
- [x] `emision` (parser field) vs `self_emision` (computed `emision * 2.5`) — documented in data.h comment

### Lighting
- [ ] `apply_al()` normalizes RGB to 0-1 and back — lossy for dark colors. Fix: use float colors internally
- [ ] `difuse_light()` mixes `obj->rgb` with light `rgb_inty` via integer division → banding
- [ ] `compute_emissive_light()` passes `emitter` as `self` to `data_shadow` but `parent` logic is broken
  (parent is an incrementing counter, not inherited by caps/sides)

### Geometry
- [ ] `check_obj` cone hack: `obj->size += 35` — magic number, document as "cone angle correction"
- [ ] `half_angle` computed AFTER `size += 35` — coupling hidden between `check_obj` and `init_obj_normi`
- [ ] `cone_calc` formula uses `calcs.k = cos_half²` — verify against standard cone equation
- [ ] `hit_cy` uses `EPSILON` for height bounds but `EPSILON` is `1e-3` — too large for thin cylinders?

### Memory & Safety
- [ ] `last_exit()` calls `mlx_terminate()` before freeing objects — potential use-after-free if MLX callbacks fire
- [x] `er()` function calls `exit()` but returns `int` — changed to `_Noreturn void`
- [ ] `t_data->args` (global args from parser) — stored in `data->args`, freed in `er()`. Move to local var
- [ ] Add Valgrind/ASan run to CI or manual testing checklist
- [ ] `init_raysc()` (console render) — same signature as `init_rays()` but different behavior, document
- [x] `debug_info.c` functions unused in release — wrapped print functions in `#ifdef DEBUG`

---

## 🔵 LOW — Polish & Consistency

- [ ] Rename `t_slight` → `t_light` (it's not just "spot", it's point light)
- [ ] Rename `t_alight` → `t_ambient`
- [ ] Rename `t_rgb` → `t_color` or keep `t_rgb`
- [ ] Split `data.h` (382 lines) → `types.h` + `scene.h` + `material.h`
- [ ] Split `render.h` (269 lines) → `ray.h` + `viewport.h` + `intersect.h` + `lighting.h`
- [x] Document scene `.rt` file format in `doc/FORMAT.md`
- [x] Add `doc/ARCHITECTURE.md` explaining the ray tracing pipeline

- [ ] Console enum `t_btn_name` has intentional value collisions — document or separate
- [ ] Console `t_img_btn.row1[200]` etc. — replace fixed-size buffers with dynamic strings
- [ ] Error messages in Spanish → English (or keep bilingual)
- [ ] `debug_info.c` functions unused in release — wrap in `#ifdef DEBUG`

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
4. **~~Remove dead code~~** (`m_trace`, `trace_flag`, `specular_light`) ✅
5. **~~Fix remaining console bugs~~** (NULL checks, dead code) ✅
6. **~~Refactor material system~~** (merge init_materials + init_materials_render) ✅
7. **Create reference scenes** (3 mandatory scenes from subject)
8. **Documentation** (FORMAT.md, ARCHITECTURE.md)
9. **Polish** (rename types, split headers, cleanup)

