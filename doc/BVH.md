# BVH — Bounding Volume Hierarchy

## ¿Qué es?

Una estructura de aceleración espacial que reduce el coste de intersección
rayo-escena de **O(n)** a **O(log n)**, donde n es el número de objetos.

En lugar de testear cada rayo contra **todos** los objetos secuencialmente,
el BVH organiza los objetos en un árbol de cajas envolventes (AABB).
El rayo solo se testea contra los objetos cuyas cajas intersecta.

## Cómo funciona

### 1. Construcción (build_bvh)

```
Por cada objeto → calcular AABB (Axis-Aligned Bounding Box)
Crear array de punteros a objetos + sus centroides

build_recursive(objetos[], inicio, fin):
  1. Calcular AABB combinada de todos los objetos en el rango
  2. Si nº objetos ≤ BVH_LEAF_SIZE (4) → crear nodo hoja
  3. Si no:
     a. Encontrar el eje más largo de la AABB
     b. Ordenar objetos por centroide en ese eje (partición rápida)
     c. Dividir en la mediana
     d. Recursión: construir hijo izquierdo y derecho
```

**Complejidad de construcción:** O(n log n)

### 2. Recorrido (bvh_traverse)

```
Pila explícita de 64 elementos (evita recursión)
Empujar nodo raíz

Mientras pila no vacía:
  Pop nodo
  Si rayo NO intersecta AABB del nodo → skip
  Si nodo hoja:
    Testear cada objeto individualmente (hit_sp, hit_pl, etc.)
    Mantener el más cercano (t < t_min actual)
  Si nodo interno:
    Empujar hijo izquierdo y derecho
```

**Complejidad de recorrido:** O(log n) en media

### 3. AABB por tipo de objeto

| Tipo | Cálculo |
|------|---------|
| Esfera | `centro ± radio` en los 3 ejes |
| Cilindro | Dos discos (base y tapa) de radio R, orientados según axis |
| Cono | Disco en la base (radio R) + punto del ápice |
| Plano | Caja "infinita" (±10000) |
| Cubo | `pos ± half_size` |

### Estructuras de datos

```c
typedef struct s_aabb {
    t_v3 min;  // esquina inferior
    t_v3 max;  // esquina superior
} t_aabb;

typedef struct s_bvh_node {
    t_aabb box;       // caja del nodo
    int    left;      // índice hijo izq (-1 si hoja)
    int    right;     // índice hijo der (-1 si hoja)
    int    obj_start; // primer objeto en hoja
    int    obj_count; // nº objetos en hoja
} t_bvh_node;
```

Los nodos se almacenan en un array plano (`t_bvh_node *bvh_nodes`) para
mejor localidad de caché. Los objetos se reorganizan en un array de punteros
(`t_obj **bvh_objs`) durante la construcción.

## Impacto en rendimiento

Para una escena con N objetos:
- **Sin BVH:** N tests de intersección por rayo
- **Con BVH:** ~log₂(N) tests de AABB + ~4 tests de objeto por rayo

Para la escena `figuras_pov.rt` con 26 objetos:
- Sin BVH: 26 tests/rayo
- Con BVH: ~5 tests AABB + 4 tests objeto ≈ 9 tests/rayo (**~3× más rápido**)

Para escenas con 100+ objetos, la mejora es de **10-100×**.

## Limitaciones

- Planos y SIDE tienen AABB "infinita" → siempre se testean si el rayo
  atraviesa esa región del espacio
- Tras mover objetos en la consola, el BVH **no se reconstruye** automáticamente.
  Solo se construye una vez en `init_all()`.
- La profundidad máxima de pila es 64 (suficiente para árbol balanceado
  de hasta 2^32 nodos)
