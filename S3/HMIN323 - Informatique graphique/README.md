# HMIN323 - Informatique graphique

## Table des matières

[TOC]

## Informations

- [Planning](https://moodle.umontpellier.fr/mod/page/view.php?id=190260)

### Examens

- **Note finale** : 60% Exam + 40% TP

### Ressources

- [Cours et TDs-TPs](https://moodle.umontpellier.fr/course/view.php?id=5844)

## GLSL : Opengl Shading Langage

### Ressources

- [GLSL overview vidéo](https://www.youtube.com/watch?v=uOErsQljpHs)
- [GLSL langage wiki](https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language)
- [GLSL functions](https://www.shaderific.com/glsl-functions)

### Définitions

**OpenGL** : interface de programmation sous forme de machine à état permettant de communiquer avec le GPU.

**Shader** : petit programme qui s’exécute sur le GPU permettant de paralléliser facilement les calculs sur des points.

### Langage

GLSL est un mélange entre le C et le C++. Il reprend en grande partie la même syntaxe et les mêmes types avec quelques ajouts et/ou modifications.

#### Types

- **Scalaires** : `bool, int, uint, float, double`
- **Vecteurs** : `bvecN, ivecN, uvecN, vecN, dvecN` (avec $$N \in [2, 4]$$)
- **Matrices** : `matNxM, matN` (avec $N, M \in [2, 4]$)
- **Opaques** : `sampler, image, struct, array` (sampler == texture)

#### Fonctionnalités

- **Swizzling** : `vec4 v4 = other_vec.xyxx; vec2 v2 = other_vec.yx ` (on peut faire n'importe quel combinaison de xyzw)
- **Swizzle masks** : `v.xyzw == v.rgba == v.stpq;` (les valeurs sont identiques mais cela permet de rendre plus lisible le code. On utilise `.rgba` pour les couleurs et `.stpq` pour les textures)

### Opengl Shaders

#### Vertex Shader

Il traite les sommets (Projection / Déplacement).

```glsl
#version 130 // Compatible avec OpenGL 3.0

uniform mat4 MVP;

// Remplacer "attribute" par "in" quand GLSL version > 130

attribute vec3 v_position; 
attribute vec3 v_color;

// Remplacer "varying" par "out" quand GLSL version > 130

varying vec3 color;

void main()
{
	// Transform all vertices with Model View Projection matrix.
    gl_Position = MVP * vec4(vertex_position, 1.0);
    
    // Passage de la couleur v_color au fragment shader.
    color = v_color;
}
```

#### Fragment Shader

Il traite les pixels (Couleur / Plaquage de textures).

```glsl
#version 130 // Compatible avec OpenGL 3.0

varying vec3 color;

void main()
{
	// Colorize all triangles with the same color
    gl_FragColor = vec4(color, 1.0);
}
```



## Maillages

### Acquisition

- Scanners
- Tomographie
- Contours
- Nuages de points

### Analyse

- 1-voisinage : sommets reliés au sommet v par un arête
- Valence : taille du 1-voisinnage
- Maillage régulier : tous les sommets ont une valence régulière
- Maillage semi-régulier : des sommets exeptionnels on une valence irregulière

### Manipulation

- Simplification (traitement efficace de géométrie, level-of-detail, compression)
  - Partitionnement (OCS : Out-of-Core Simplification (grille), VSA : Variational Shape Approximation)
    1. Partitionner le maillage
    2. Définir sommet représentant par partition
    3. Trianguler l'ensemble des sommets représentant
  - Décimation (MO : Mesh Optimization, QEF : Quadric Error Function)
    1. Définir importance par sommet : rajoute dans une fille prioritaire de traitement les sommets
    2. Supprimer le sommet de moindre importance
    3. Continuer jusqu'a avoir le nombre de sommets voulu
- Paramétrisation (mettre à plats un maillage pour plaquer des textures)
- Remaillage
- Chanfreinage : ajout de face en replaçant un sommet ou une arete
- Mesh smoothing : filter out hight frequency mesh
  - fourier transform filtering
  - Laplacian smoothing
  - Taubin smoothing
  - Cotangent Laplacian smoothing

### Structures de données

- Maillages indexés : Simple mais pas d'accès direct et complet aux voisins d'un sommet
- WingedEdge : (problème de la navigation : **ambiguité sur orientation faces**)
  - vertex_front
  - vertex_back
  - face_left
  - face_right
  - arete_front_left
  - arete_front_right
  - arete_back_left
  - arete_back_right
- HalfEdge : désambiguité sur l'orientation (problème representation à simplifiée encore)
  - to_vertex
  - next_half_edge
  - (optionnal) previous_half_edge
  - opposite_half_edge
  - face
- Cartes combinatoires : (on ne prend que les brins entre les faces de l'objet)
  - 2-cartes = triplet (B, Beta1, Beta2)
    - B : ensemble fini de brins
    - Beta1 : permutations $ \forall b, \exist p, \beta_1(b)^p = b $
    - Beta2 : involution avec ou sans point fixe $ \forall b, \beta_2(\beta_2(b))) = b $  ; si Beta_2(b) = b alors b est un point fixe par Beta_2
- Cartes combinatoires généralisées
  -  Brin :
    - Brin* alpha[4]
    - Point* p
    - Bool marked
- quad-tree :
- kd-tree

### Propriété

- Dualité : on prend le barycentre des faces puis on trace un maillage en reliant les barycentres  diagramme de Voronoi
- Fermeture : pas de bords = toute âretes partagé par 2 triangles
- Manifold : aretes incidentes 1/2 faces, sommets incidents 0 ou 2 arêtes
- Orientation : compatible si les faces sont inversée dans l'ordre de parcours (non orientable : ruban de Möebius, bouteille de klein)
- Euler Poincaré : S - A + F = X = 2 (1 - g)
  - S : nombre de sommets
  - A : nombre d'arêtes
  - F : nombre de faces
  - g : nombre de trous dans un objet fermé

## TP2 : Maillages

- Calculer normales par faces
- Calculer 1-voisinage sommets
- Calculer valence sommets

## TP3 : Gmap

- Exercice sur gmap cours

## TP4 : Lissage

- Calculer opérateur Laplacien uniforme
- Calculer opérateur Laplace-Beltrami
- Maillages equi = bonne qualité