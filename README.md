# RT — Ray Tracer (42 School)

A Monte Carlo path tracer written in C23 with physically-based rendering.

![image](https://github.com/user-attachments/assets/3687a228-0729-4f0b-bcaf-bfcdf1d3aae0)

### Build & Run

```bash
make                    # release: -O3 -flto -march=native
make debug              # debug: -g3 -O0 -fsanitize
./rt scenes/figuras_pov.rt
```

### Controls

| Key | Action |
|-----|--------|
| `SPACE` | Toggle single render / progressive accumulation |
| `C` | Open/close interactive console |
| `Click` | Open console + interact with buttons |
| `Arrows` | Move camera (console mode) |
| `Ctrl+↑/↓` | Zoom FOV (console mode) |
| `ESC` | Quit |

### Features

- **Path tracing** — 5 bounce Monte Carlo integration
- **4 base geometries** — sphere, plane, cylinder, cone
- **Cube** — decomposed into 6 bounded faces
- **Materials** — diffuse, metallic (GGX), mirror, glass (Fresnel), emissive, checkerboard
- **Textures** — PNG texture mapping with UV coordinates
- **Bump mapping** — normal perturbation from height maps
- **Depth of field** — thin-lens camera with configurable aperture & focus distance
- **Lighting** — ambient + multiple point lights + emissive area lights
- **Shadows** — shadow rays with occlusion testing
- **Specular highlights** — Phong model per material
- **BVH** — bounding volume hierarchy acceleration
- **Multi-threading** — 6-thread pthread pool
- **Progressive rendering** — sample accumulation over time
- **Interactive console** — live edit camera, lights, objects, materials
- **Scene files** — `.rt` format with material suffixes

### Scene Examples

```bash
./rt scenes/subject_1.rt      # 4 base objects + 2 lights
./rt scenes/subject_2.rt      # Same scene, rotated camera
./rt scenes/subject_3.rt      # Shadow mixing demo
./rt scenes/03_glass_galore.rt # Glass + Fresnel
./rt scenes/11_depth_of_field.rt # DOF demo
```
