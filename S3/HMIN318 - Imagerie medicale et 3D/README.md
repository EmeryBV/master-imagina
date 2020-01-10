# HMIN318 - Imagerie médicale 3D

## Table des matières

[TOC]

## Informations

- [Planning](https://moodle.umontpellier.fr/mod/page/view.php?id=190283)

### Examens

- **Note finale** : 60% Exam + 40% (Projet + TP)

### Ressources

- [Cours et TDs-TPs](https://moodle.umontpellier.fr/course/view.php?id=5849)
- [Manuel de CImg](http://cimg.eu/CImg_reference.pdf)

### À savoir

Choix des projets du 26 septembre au 3 octobre
- Acquisition 3D
- Visualisation 3D
- Segmentation 3D

- Recalage 3D
- Analyse 3D

## Image 3D

### Définition

- Matrice à 3 dimensions
- Contient un ensemble d'images 2D
- Le plus petit élément de l'image 3D est un Voxels

### Fabrication

- Découpe en tranches
- Acquisition volumique (X-Ray, Échographie, IRM, etc...)

- Discrétisation volumique (Voxelisation de surfaces, maillages etc...)

### Stockage

- Info stockage dans le header de l'image (12 bits ou 8 bits) souvent sur 2 octets
- Attention codage BigEndian / LittleEndian : $ 1000 = 256 \times 3 + 232 $ (03 E8 ou E8 03...)
- Format standard : DICOM (Digital Imaging and Communications in Medicine)

### Visualisation

- Visualisation par morceaux : coupe en 3 plan orthogonaux

- Visualisation entière : définition d'une couleur et opacité pour chaque voxels en fonction de leurs intensités.
  - On lance des rayons depuis le point de vue pour visualiser le volume
  - Plus le rayon pénètre dans le volume plus l'opacité réduit jusqu'à atteindre 1 d'opacité
  - Définition d'une courbe d'opacité sur nos gradient

## Segmentation

- **Segmentation** : Subdivision de l'image en régions 
- **Classification** : ranger les pixels dans des régions
- **Labélisation** : Assigner un label à une région

- **Voisinage** : neighborhood = connectivity
  - 2D : 4-neighborhood (aretes) / 8-neighborhood (sommets)
  - 3D : 6-neighborhood (faces) / 18-neighborhood (aretes) / 26-neighborhood (sommets)

### Philtres

- **lineaires** :
  - **Smoothing** : $ M = {1 \over |M|}.\begin{bmatrix}1 & 1 & 1\\1 & 1 & 1\\1 & 1 & 1\end{bmatrix} $ (Adouci l'image / flou)
  - **Contrast** :  $ M = {1 \over |M|}.\begin{bmatrix} 0 & -1 & 0\\-1 & 5 & -1 \\0 & -1 & 0\end{bmatrix} $ (Améliore les contraste)
  - **Contouring** : $M = \begin{bmatrix} 0 & 1 & 0\\1 & -4 & 1 \\0 & 1 & 0\end{bmatrix}$  où  $M = \begin{bmatrix} 1 & 1 & 1\\1 & -8 & 1 \\1 & 1 & 1\end{bmatrix}$(Approximation du laplacien)
- **Non lineaires** :
  - **Median** :  $ M = \begin{bmatrix} 5 & 6 & 7\\6 & 111 & 8\\7 & 8 & 9\end{bmatrix} \rightarrow \begin{bmatrix} 5 & 6 & 7\\6 & 7 & 8\\7 & 8 & 9\end{bmatrix}$ (Enlève le "bruit poivre et sel")

### Seuillages

#### Otsu automatic thresold algorithm 

This threshold is determined by minimizing intra-class intensity variance, or equivalently, by maximizing inter-class variance

**Algorithm** : ($w_i$ = weight, $t$ = threshold, $\sigma^2_i$ = variance of class, $µ$ = class mean)

1. Compute histogram and probailities of each intensity level
2. Set up initial $ w_i(0) $ and $ µ_i(0) $
3. Step through all possible thresholds $ t = 1, ... $ maximum intensity
   1. Update $w_i$ and $µ_i$
   2. Compute $ \sigma^2_b(t) $
4. Desired threshold corresponds to the maximum $\sigma^2_b(t)$

### Algorithmes

#### Livewire

Segmentation interactive : définir un fonction de coût pour passer d'un pixel à un autre (coût faible si les pixels sont sur un contour)

1. Fixer un pixel de départ O
2. Calculer le plus cours chemin pour tous les pixels P (Dijkstra)

#### Watershed

Inondation de l'image en partant d'une intensité i = imin jusqu’à une intensité max imax

```
Tant que i < imax
	Pour tout voxels V d'intensité i sans label
		si V connecté à des voisins tous labellisés Lj
			V labelisé Lj
		si aucun voisin de V labellisé
			V crée nouveau label Lk
		Si V est connecté à des voisins de labels différents
			V labellisé W (watershed = ligne de partage des eaux)
	i = i + 1
```

## Recalage

Correspond à une simple correspondance de points dans l'espace selon des critères.

- **Utilité** : Diagnostic / Étude pathologie / Chirurgie guidées par image / Comparaison de patients

- **Formule** : $\min f(I_1, t(I_2)) \quad t \in T$
  - $I_1$ et $I_2$ images à recaler
  - $t$ : transformation
  - $T$ : ensemble des transformations possibles
  - $f$ : critère de dissimilarité ($\min$) ou similarité ($\max$)

### Transformations

- **Rigide** : Rotation et translation
- **Affine** : Oblique et mise à l'échelle

### Correspondance

- Plus proche voisins : selon plusieurs critères (intensité, position, ...)
- Ancres : fixer à la main
- Calculer moyenne de centroïdes (groupes de points)
- Points caractéristiques

### Critères de similarité

- **Histogramme conjoint** : $ H_{i,j}(i, j) = Card\{(x, y)\space| \space I(x, y) = i \space et \space J(x, y) = j \} $
  - Si $i$ et $j$ $\in [0, 255]$ taille de $H_{i, j} =$ 256x256 

## Image 4D

Empilement de plusieurs images 3D en fonction du temps