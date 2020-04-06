# Compte Rendu N°1 (3 Mars 2020)

## Étude des fonctionnalités potentiellement nécessaires à la création de l'application

- Lecture de maillages
- Affichage de maillages
- Analyse de maillages
- Segmentation de maillages
- Comparaison de maillages
- Fusion de maillages
- Découpage de maillages
- Création de maillages
- Région Growing
- Reconstruction / Blending
- Seuillage
- Kd-tree
- Support tablette (avec pression du stylet)

## Recherche des technologies adéquates

### Bibliothèques

- Trimesh : lecture et traitement de maillages
- PCL : traitement de nuages de points et de maillages
- CGAL5 : lecture, traitement et génération géométriques (modulaire, générique et complet)
- Qt5 : fenêtrage complet et portable
- Imgui : fenêtrage OpenGL portable (pratique pour le prototypage de fonctionnalités)
- LibQGlViewer : manipulation et visualisation de maillages

### Outils

- Blender : génération de maillages
- MeshLab : manipulation et visualisation de maillages

### Sélection

Le choix des technologies de développement s'est finalement porté sur CGAL5 + Qt5. Ces bibliothèques sont moderne, mises à jour régulièrement avec des documentations plutôt complètes (surtout pour Qt5).

- CGAL5 : dispose d'un grand nombre de modules qui couvrent les fonctionnalités géométriques nécessaires pour notre application. Cette bibliothèque possède également des interfaces pour communiquer avec Qt et libQGlViewer ce qui est idéal pour nous.
- Qt5 : permet de créer des applications fenêtrées portables supportant les tablettes avec pression du stylet.

## Mise en place des technologies

- Création d'un répertoire public pour accéder au code source de l'application https://github.com/todorico/surgery-viewer
- Création de CMakelists.txt pour la compilation efficace de l'application en cross-platform avec CMake
- Création d'une application de visualisation simple de maillages
- Création d'un programme permettant de générer des maillages à l'aide d'équations mathématiques

## Travail à finir pour la semaine suivante

- Création de maillages simple à l'aide de Blender pour effectuer des tests de blending / interpolation
- Tester différentes méthodes sur ces maillages pour tenter pouvoir couper des zones d'intérêts de manière cohérente (distance, seuillage, segmentation) et adopter une approche en fonction des résultats
