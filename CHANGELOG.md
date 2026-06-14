# CHANGELOG

## [0.2.0] — 2025-06-14 — Build System & Strict Warnings Overhaul

### Build System
- **Release flags:** `-O3 -flto -march=native` (binary shrunk from 1.1MB → 695KB)
- **Debug target:** `make debug` → `-g3 -O0 -fsanitize=address,undefined,leak,bounds`
- **New targets:** `make help`, `make rebug`
- **GLFW linking:** Fixed to use built-in `libglfw3.a` + `-lX11 -lXext`
- **Strict warnings (16 flags):** `-Wshadow -Wunused -Wformat=2 -Wundef -Wstrict-prototypes -Wmissing-prototypes -Winit-self -Wimplicit-fallthrough=3 -Wcast-align -Wswitch-default -Wswitch-enum -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wstack-protector -fstrict-aliasing`

### Strict Flag Compliance
- Added `const` qualifiers: `er()`, `ft_strcmp()`, `bts[]` string arrays
- Added `static` to 14 internal functions missing prototypes
- Fixed sign-conversion casts for `ft_substr`, `mlx_new_image`, `bm_size`/`tx_size`
- Fixed `if/else` misleading indentation with braces
- Fixed `return (expr)` in void functions → `expr; return;`
- Fixed `10.0f` → `10.0` double promotions in float math
- Fixed shadowed parameter `right` → `rgt`

### Bugfixes (continued from 0.1.x)
- **`parse_tx`:** was setting `bm_size` instead of `tx_size` (copy-paste bug)
- All 0 errors, 0 warnings on `make` and `make debug`

---

## [0.1.0] — 2025-06-14 — Critical Bugfixes & Refactor

### Critical Bugs Fixed

| Bug | File | Description |
|-----|------|-------------|
| Refraction formula | `materials_utils.c:42` | Used `eta²` for direction vector instead of `eta` |
| Uninitialized `uv` | `calcs.c:114` | `uv` read before assignment when no texture → UB |
| Cone intersection | `cone.c:63` | Only checked `quad.t1`, never `quad.t2` |
| Random hemisphere | `calcs_utils.c:23` | `rand()/FLT_MAX` instead of `rand()/RAND_MAX` |
| Shadow caps skip | `illumination.c:79` | All CAP objects unconditionally skipped in shadow rays |
| Serialized threads | `calcs.c:120` | Mutex locked around entire `path_trace` → single-threaded |
| Frame fallback bug | `init_side_utils.c:22` | Fallback cross used same vector `(1,0,0)` twice |
| Camera debug offset | `parse_acl.c:72` | `pos.z += 200` debug artifact removed |
| Cube validation | `parse_checker.c:60` | Checked `cube_size.x` 3x instead of x, y, z |
| Diffuse on bumpmaps | `calcs.c:50` | Returned black for all bump-mapped objects |
| Texture type limit | `calcs.c:91` | Textures only worked on spheres, not other types |
| Material init override | `init_materials.c` | `init_df()` reset roughness/absorption overriding parser |
| Checkerboard coords | `checker_board.c` | Used world `x,z` instead of local tangent space |
| Memory leak (x2) | `render.c`, `console/render/render.c` | `vp`/`rays` not freed on error paths |
| `parse_tx` wrong field | `extra_funcs.c:84` | Set `bm_size` instead of `tx_size` |

### Build Fixes
- Fixed `calloc(nmemb, size)` argument order in libft and project (GCC 14 `-Wcalloc-transposed-args`)

### Refactors
- **Include paths:** Standardized from `../inc/miniRT.h` / `../../inc/miniRT.h` → `"miniRT.h"`
- **Variable naming:** Replaced cryptic arrays:
  - `dirb[4]` → `base_color`, `direct_light`, `indirect_light`, `result`
  - `dnrxy[3]` → `denom`, `numer`, `result`
  - `idyx[3]` → `thread_id`, `y`, `x`
  - `t[2]` → `t_min`, `max_dist`
  - `hits_lp[2]` → `hit_point`, `local_point`

---

## [0.0.1] — Prior History (pre-refactor)

- Initial ray tracer implementation
- Console UI for live scene manipulation
- Parser for `.rt` scene files
- MLX42 integration
- Pthread rendering
