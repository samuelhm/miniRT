# Scene File Format (`.rt`)

A scene file defines all objects, lights, and the camera for the ray tracer.
Lines starting with `#` are comments (ignored).

## Elements

### Ambient Light (mandatory, exactly one)
```
A    br r,g,b
```
- `br` — brightness (0.0 to 1.0)
- `r,g,b` — RGB color (0-255)

### Camera (mandatory, exactly one)
```
C    x,y,z  nx,ny,nz  fov  [focus_dist]  [aperture]
```
- `x,y,z` — position
- `nx,ny,nz` — orientation vector (normalized, each component 0.0–1.0)
- `fov` — field of view in degrees (0–180)
- `focus_dist` — optional, focal distance for depth of field
- `aperture` — optional, aperture radius for depth of field

### Point Light (at least one)
```
L    x,y,z  br  r,g,b
```
- `x,y,z` — position
- `br` — brightness (0.0 to 1.0)
- `r,g,b` — RGB color (0-255)

### Sphere
```
sp   x,y,z  diameter  r,g,b  [material...]
```
- `diameter` — sphere diameter (> 0)

### Plane
```
pl   x,y,z  nx,ny,nz  r,g,b  [material...]
```
- `nx,ny,nz` — normal vector

### Cylinder
```
cy   x,y,z  nx,ny,nz  diameter  height  r,g,b  [material...]
```
- `diameter` — cylinder diameter (> 0)
- `height` — cylinder height (≥ 1)

### Cone
```
co   x,y,z  nx,ny,nz  diameter  height  r,g,b  [material...]
```
- `diameter` — base diameter (0–54)
- `height` — cone height (≥ 1)

### Cube
```
cu   x,y,z  nx,ny,nz  width  height  depth  r,g,b  [material...]
```
- `width`, `height`, `depth` — dimensions (> 0 each)

## Material Suffixes

After the color, optional 2-char material suffixes can be added:

| Suffix | Material | Extra args |
|--------|----------|------------|
| `mt` | Metallic (rough reflection) | — |
| `gl` | Glass (refraction + reflection) | — |
| `mr` | Mirror (perfect reflection) | — |
| `cb` | Checkerboard (planes only) | `scale r,g,b` |
| `em` | Emission (light source) | `intensity` (0.0–1.0) |
| `bm` | Bump map (spheres only) | `size path.png` |
| `tx` | Texture (spheres only) | `size path.png` |

Checkerboard scale must be between 0.001 and 0.991.

## Example

```
# Ambient
A    0.3    200,200,255
# Camera
C    0,5,-30  0,-0.15,0.98  70
# Lights
L    -15,15,5  0.7  255,255,255
L    15,15,5   0.7  255,255,255
# Objects
sp   -5,0,0    8    255,80,80
pl   0,-5,0    0,1,0  100,100,255  cb 0.5 60,60,150
cy   5,-1,0    0,1,0  5 12  80,255,80  mr
co   0,1,10    0,1,0  6 10  255,255,80
```
