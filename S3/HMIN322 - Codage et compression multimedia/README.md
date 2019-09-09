# HMIN322 - Codage et compression multimedia

## Table des matières

[TOC]

## Informations

### Examens

- **Note finale** : 33% CC + 66% Exam

### Ressources

- [Cours et TDs-TPs](https://www.lirmm.fr/~wpuech/enseignement/master_informatique/Compression_Insertion/)

## Sécurisation des données

- **Compression** : suppression des redondances en réduisant la taille de l'image

- **Chiffrement** : suppression des redondances sans réduire la taille de l'image

**Remarque** : on ne peut pas chiffrer une image puis la compresser (plus de redondances). Les algorithmes utilisé sont donc des hybridations d'algorithmes de compression et de chiffrement.

## Insertion de données cachées

- Invisible
- Résiste aux transformations
- Contrainte sur la longueur maximum du message
- Résiste aux attaques
- Applicable dans des applications en temps réel 

![triangle](README.tex.assets/triangle.svg)

<img src="README.tex.assets/triangle.svg?invert_in_darkmode" align=middle/>

<img src="README.tex.assets/triangle.svg" align=middle/>

## Hachage

- Chiffrement de n'importe quelle donnée en une petite signature de taille fixe
- À la moindre différence entre les deux donnée la signature deviens totalement différente

### Caractéristiques d'une image

- Forme

- Texture
- Couleurs

- **Symmetric encryption by block** : DES, TEA, AES laisse des traces de l'ancienne image

## Format des images et colorimétrie

Grande diversité d'image <img src="svgs/4fb92cbc0a3b3a976a9903e02e19ad5a.svg?invert_in_darkmode" align=middle width=16.438455pt height=14.15535pt/> difficulté à créer un compresseur efficace universel

### Codage source

- Compression
- Chiffrement
- Insertion de données cachées

### Format d'un fichier image

- **Header**
  - Code : Magic number
  - Format d'image en pixel : L x H
  - Taille d'un pixel : 1 bit (binaire), 8 bits (monochrome), 24 bits (3 x 8 bits : vraies couleurs)

- **Données images**
  - Données des pixels
  - Ordre de lecture des pixels
  - Structures complexes

- **Footer**
  - Informations supplémentaires de l'image

**Exemple** : Header pour le format PNM (Portable Any Map) 

```
P5
512 512
255
# Commentaires
```

## Compression sans perte

- **TGA** : Algorithme RLE
- **GIF** :  Algorithme LZW
- **PNG** : Algorithme LZ77
- **TIFF** : Algorithme RLE + Codage prédictif
- **JPEG** : Compression sans perte possible : Codage prédictif
- **JPEG2000** : JPEGLS



## TP1 - Palette

réduction des couleurs d'une image en une palette de couleurs pertinente



## TP2 - Espace couleur

Image RGB au format N x M <img src="svgs/bed3268d80a96d180edd6863c11d1a83.svg?invert_in_darkmode" align=middle width=16.438455pt height=14.15535pt/> 3 x M x N pixels

en sortie on veux 1.5 x M x N pixels (Taux de compression à deux)

ré-échantillonnage en prenant seulement 2 composantes

<img src="svgs/e9738ed3e60073ce799e230f872936fc.svg?invert_in_darkmode" align=middle width=175.688205pt height=30.64842pt/>

**1) Image compressé :**

<img src="svgs/ce77ee2972624d3b3a169f890210524c.svg?invert_in_darkmode" align=middle width=72.188325pt height=28.67073pt/> avec ré-échantillonnage

<img src="svgs/3bb7a72647a41321f4df3f2c8c4e5560.svg?invert_in_darkmode" align=middle width=47.767005pt height=22.46574pt/>

<img src="svgs/34debb8c62e9ab367398ce75c00b8919.svg?invert_in_darkmode" align=middle width=72.87324pt height=28.67073pt/> avec ré-échantillonnage



<img src="svgs/427be8158fb40c56d48a1f534ad54429.svg?invert_in_darkmode" align=middle width=73.01877pt height=28.67073pt/>

<img src="svgs/e6ff723d461a6a2d0ac60234e48aad39.svg?invert_in_darkmode" align=middle width=184.083405pt height=33.45969pt/>

<img src="svgs/6eef0762e2fca70fbda934973470e0c6.svg?invert_in_darkmode" align=middle width=249.251805pt height=27.77577pt/>



**2) RGB <img src="svgs/bed3268d80a96d180edd6863c11d1a83.svg?invert_in_darkmode" align=middle width=16.438455pt height=14.15535pt/> YCrCb :**

Même chose en sacrifiant Cr et Cb

**Conclusion** : cette compression marche mieux en YcrCb qu'en RGB



## Théorie de l'information

## Compression sans perte

## Compression avec perte

