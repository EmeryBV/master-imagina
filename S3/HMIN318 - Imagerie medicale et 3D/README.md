# HMIN318 - Imagerie médicale 3D

## Table des matières

[TOC]

## Informations

### Examens

- **Note finale** : 60% Exam + 40% (Projet + TP)

### Ressources

- [Cours et TDs-TPs](https://moodle.umontpellier.fr/course/view.php?id=5849)
- [Manuel de CImg](http://cimg.eu/CImg_reference.pdf)

### À savoir

- Choix de projet du 26 septembre au 3 octobre
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
- Attention codage BigEndian / LittleEndian : $ 1000 = 256 \times 3 + 232 $

## Visualisation volumique

- Définir couleur et opacité pour chaque voxels en fonction de son intensité
- On lance des rayons depuis le point de vue pour visualiser le volume
- Plus le rayon pénètre dans le volume plus l'opacité réduit jusqu'à atteindre 1 d'opacité
- Définition d'une courbe d'opacité sur nos gradient