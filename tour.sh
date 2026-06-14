#!/bin/bash
# Tour de escenas — cada escena se abre al cerrar la anterior

SCENES=(
	"01_classic_spheres"
	"02_checkerboard_room"
	"03_glass_galore"
	"04_metal_workshop"
	"05_emissive_neon"
	"06_bump_mapped_spheres"
	"07_mirror_hall"
	"08_cone_forest"
	"09_cylinder_temple"
	"10_cube_abstract"
	"11_depth_of_field"
	"12_sunset_room"
	"13_disco_lights"
	"14_minimal_art"
	"15_textured_spheres"
	"16_shadow_play"
	"17_cylinders_cubes"
	"18_glass_prism"
	"19_landscape"
	"20_showcase"
	"normal"
	"pov"
	"sky"
)

for scene in "${SCENES[@]}"; do
	echo "=== $scene ==="
	./rt "scenes/$scene.rt" || break
done
