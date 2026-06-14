# RT — Subject Compliance (v4.1)

## Mandatory Part (Chapter V)

| # | Requisito | Cómo se cumple |
|---|-----------|----------------|
| 1 | **Binary name `rt`** | `Makefile:13` — `NAME := rt` |
| 2 | **Code in C (latest standard)** | `-std=gnu23`, compila con `-Wall -Wextra -Werror` + 14 flags extra, 0 warnings |
| 3 | **Ray tracing method** | Monte Carlo path tracer con hasta 5 rebotes (`MAX_DEPTH=5`). La función `trace_ray()` en `src/render/calcs/calcs.c:134` dispara `path_trace()` recursivo. |
| 4 | **4 objetos geométricos base** | **Sphere** (`hit_sp`, `src/render/obj/intersections.c:31`), **Plane** (`hit_pl`, `intersections.c:67`), **Cylinder** (`hit_cy`, `src/render/obj/cylinder.c:73` + caps), **Cone** (`hit_cone`, `src/render/obj/cone.c:55` + cap). Intersectan con ecuación analítica (no triángulos). |
| 5 | **Translation y rotation** | **Translación**: posición (x,y,z) de cada objeto, cámara y luz se especifica en el `.rt` y se modifica en runtime desde la consola interactiva (botones ±, `OBJPLUS=0.5`, `SPOTPLUS=0.5`, `CAMPLUS=0.5`). **Rotación**: el vector `axis` (nx,ny,nz) define la orientación de cylinder, cone, cube y plane; modificable desde la consola con `OBJAXISP=0.1` por componente. La cámara también rota via su `axis` (botones ± en consola) y teclado (Ctrl+↑↓ para FOV). Ver `src/console/console/console_click_obj.c`, `console_click_cam.c`, `console_click_slight.c`. |
| 6 | **Camera changeable** | Posición y dirección configurables en `.rt` (`src/parser/obj/parse_acl.c:61`). En runtime: teclado (flechas mueven posición, Ctrl+↑↓ cambia FOV) + consola interactiva (botones ± para pos, axis, FOV, aperture, focus). |
| 7 | **Redraw sin recalcular (expose)** | MLX42 usa texturas OpenGL — `data->img` persiste en GPU y se redibuja automáticamente al exponer la ventana. Además se cachea `data->img_last` como copia CPU tras cada `render_one`. El `mlx_close_hook` garantiza limpieza al cerrar. Al hacer resize, se dispara re-render completo (correcto, las dimensiones cambiaron). |
| 8 | **Light: different brightness** | `br` parametrizable en ambient light (`A br r,g,b`) y cada point light (`L x,y,z br r,g,b`). Ajustable en runtime desde la consola (±0.02, 0-1). |
| 9 | **Light: shadows** | `data_shadow()` en `src/render/illumination/illumination.c:70` — dispara rayos de sombra y testea oclusión contra todos los objetos. Objetos `GL` (glass) no proyectan sombra. |
| 10 | **Light: multi-spot** | Lista enlazada `t_slight` en `inc/data.h:146`. Sin límite de cantidad. `iter_lights()` en `src/render/calcs/calcs_utils.c:96` itera todas. |
| 11 | **Light: shine effect** | Phong specular inline en `iter_lights()` línea 110-119: refleja dirección de luz, dot con view direction, eleva a `shininess`, escala por `specularity` del material. Parámetros configurables por material (diffuse: 0.1/50, glass: 0.9/150, metallic: 0.9/170, mirror: 1.0/500). |

## Options (Chapter VI)

| # | Opción | Cómo se cumple |
|---|--------|----------------|
| 1 | **Ambient light** | `A br r,g,b` en `.rt`. `apply_al()` en `src/render/illumination/illumination.c:17` — `obj_color * (ambient.rgb / 255) * ambient.br`. Precomputado en `obj->a_rgb`. |
| 2 | **Direct light** | Point lights (`L`). `iter_lights()` calcula Lambert + Phong specular por cada luz. |
| 3 | **Bump mapping** | `bm size path` en `.rt`. `src/render/bump_map/bm_sphere.c` y `bm_plane.c` perturban normales usando tangentes/bitangentes. Solo esferas (parser restringe). |
| 4 | **Colour disruption** | Combinando `bm` + `tx` en un mismo objeto: el bump map perturba la normal y la textura PNG aporta el color sobre la superficie alterada. `parse_bm_and_tx()` / `parse_tx_and_bm()` en `extra_funcs_utils.c:58,70` manejan la combinación. En render, `calcs.c` aplica `texture_weight()` para hacer blend del path trace con el color de textura sobre la normal perturbada. |
| 5 | **External scene files** | `.rt` files. `parse()` en `src/parser/parser/parser.c` lee línea a línea con `get_next_line()`. |
| 6 | **Reflection** | **Mirror** (MR): `mirror_ray()` — reflexión perfecta. **Glass** (GL): `glass_ray()` — Fresnel con blend reflect/refract. **Metallic** (MT): `metallic_ray()` — GGX microfacet + Schlick-Fresnel + roughness. |
| 7 | **Transparency** | **Glass** (GL): `refract()` en `materials_utils.c:35` — refracción Snell con IOR configurable. `fresnel()` en `materials_utils.c:61` — relación reflect/transmit. |
| 8 | **Shadow mod por transparency** | Glass objects no proyectan sombra (`data_shadow()` línea 84-88 salta objetos GL). Solo GL, no sombras tintadas. |
| 9 | **Composed: cubes** | `cu` → 6 `SIDE` faces (bounded planes). `init_sides()` en `src/render/init/init_side.c:103` genera las 6 caras con bounded rectangle check. |
| 10 | **Textures** | `tx size path` en `.rt`. PNG textures con UV mapping para sphere, plane, cylinder, cone, cube. `texture_color()` en `src/render/texture/uv_map.c:35`. |
| 11 | **Limit by texture** | Objetos con textura saltan lighting y path tracing (`path_trace()` línea 146-149). Con BM+TX combinados, se aplica blend (90% path trace, 10% texture). |
| 12 | **Checkerboard** | `cb scale r,g,b` en `.rt`. `checkerboard_color()` en `src/render/color/checker_board.c`. |
| 13 | **Emission materials** | `em intensity` en `.rt`. Objetos EM emiten luz propia + actúan como area lights hacia otros objetos. |

## Bonus Features (Chapter VII)

| Feature | Descripción |
|---------|-------------|
| **Cubes** | Compuesto de 6 caras rectangulares acotadas. |
| **BVH** | Bounding Volume Hierarchy — aceleración de intersección. `src/render/bvh/`. |
| **Multi-threading** | 6 hilos pthread. `render_with_threads()` en `src/render/render/render.c:76`. |
| **Progressive rendering** | Acumulación de samples. `update_render()` en `three_renders.c:108`. |
| **GGX Metallic** | Microfacet reflection model con roughness y Schlick-Fresnel. `metallic_ray()` en `materials.c:116`. |
| **Depth of Field** | Thin-lens camera con aperture y focus distance. `apply_dof()` en `render.c:28`. Escena demo: `scenes/11_depth_of_field.rt`. |
| **Interactive Console** | UI overlay para editar cámara, luces, objetos y materiales en tiempo real. |
| **Emission area lights** | Objetos EM iluminan otros objetos (no solo self-glow). `compute_emissive_light()` en `calcs_utils.c:76`. |

## Escenas de Referencia

| Subject Figure | Archivo | Contenido |
|----------------|---------|-----------|
| VI.1 (4 objetos + 2 spots + shadow/shine) | `scenes/subject_1.rt` | sp, pl, cy, co + 2 point lights |
| VI.2 (misma escena, otro viewpoint) | `scenes/subject_2.rt` | Ídem, cámara rotada 90° |
| VI.3 (shadow mixing) | `scenes/subject_3.rt` | 3 spheres (df, mt, gl) + cy + co + 2 planos + 2 luces |

## Memoria

| Aspecto | Estado |
|---------|--------|
| `-fsanitize=address,undefined,leak,bounds` (debug) | ✅ |
| Escape limpio (`mlx_close_window` → `mlx_loop` retorna → `last_exit()`) | ✅ |
| Close button limpio (vía `mlx_close_hook` → `last_exit()`) | ✅ |
| `er()` NULL-safe | ✅ |
| 0 errores, 0 warnings (`-Wall -Wextra -Werror` + 14 flags extra) | ✅ |
