# HMIN322 - Codage et compression multimedia

## Table des matières

[TOC]

## Informations

### Examens

- **Note finale** : 60% Exam + 40% TP

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

![triangle_chiffrement](.README.assets/triangle_chiffrement.svg)

## Hachage

- Chiffrement de n'importe quelle donnée en une petite signature de taille fixe
- À la moindre différence entre les deux donnée la signature deviens totalement différente

### Caractéristiques d'une image

- Forme

- Texture
- Couleurs

- **Symmetric encryption by block** : DES, TEA, AES laisse des traces de l'ancienne image

## Format des images et colorimétrie

Grande diversité d'image $ \Rightarrow $ difficulté à créer un compresseur efficace universel

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

### Mode de transmission

#### a) Transmission séquentielle

#### b) Transmission progressive

#### Entrelacement

Données transmises en un certain nombre de passes.

$ 2 \rightarrow n $      2 : lignes paires, lignes impaires 

7 passes : (Adam 7)
$$
\begin{pmatrix}
	1 & 1110.1 & a\\
	2 & 10.1 & b\\
	3 & 23.113231 & c\\
\end{pmatrix}
$$
















```
1 & " " & 4 & " " & 1 " " & 4 & " "\\
7 & 7 & 7 & 7 & 7 & 7 & 7 & 7 \\
" " & " " & " " & " " & " " & " " & " " & " " \\
7 & 7 & 7 & 7 & 7 & 7 & 7 & 7 \\
3 & " " & 4 & " " & 3 " " & 4 & " "\\
7 & 7 & 7 & 7 & 7 & 7 & 7 & 7 \\
" " & " " & " " & " " & " " & " " & " " & " " \\
7 & 7 & 7 & 7 & 7 & 7 & 7 & 7 \\
```

1ere passe : 1 pixel (1/64)

2eme : 1 pixel (1/32)

3eme passe : 2 pixels : 1/16

4eme passe : 4 pixels

...

6eme passe : 16 pixels

7eme passe : 32 pixels



### *Par plan de bits

(Schéma)

### *Pyramide par bloc

image de 512x512 pixels $ \rightarrow $ 16x16 blocs de 32 pixels





## II - Théorie de l'information

### 1) Quantité d'information

Principes de la compression de données

claude shannon

**Théorème fondamental de Shannon** : pour une source d'information donnée et un canal d'information il existe toujours un code permettant de transmettre ce message à la capacité du canal avec un taux d'erreur binaire (**TEB**) fixé

**Message, signal** : séquence d’événements finie ou non

**Évènements** : valeurs dans un alphabet (texte, signal numérique : bits, octets), images, pixels, son (pressions accoustique, échantillons).

**Séquence finie** : nombre d'evenements longueur du message

Tous les événements d'un message sont :

- Identiques : source constante
- Indépendants (les uns des autres) : source aléatoire

Si la production d'un évenement est condition des évenements précédents :

- Les évenements corrélés
- La source est Markovienne d'ordre la longueur du message

$ \Rightarrow $ Extraction de corrélations

On découpe l'image en bloque

#### Mesure d'information

information $ \rightarrow $ probabilité

émetteur $ \rightarrow $ réception

**Exemple** Méteo : L'information à transmettre est un choix éffectué par l'emetteur entre un certain nombre d'évenements plus ou moins probables

<u>Source fini</u> : probabilité $ \rightarrow $ ocurence

source : m evenements

symbole $ \alpha \rightarrow k $ fois

- $ O(\alpha) = k $ : occurence
- $ O(\alpha) = k/m $ : probabilité

Fonction du message à transmettre ~~(et pas l'alphabet)~~ indépendant de la taille de l'alphabet

**Exemple** : m = "compression_de_donnees"

- Evenements : 22
- Alphabet : 11

$$
P(e) = { 4 \over 22 }
$$

$$
\sum p(\alpha_i) = 1
$$



l'information véhiculée par un evenement est inversement proportionnelle à son occurence.

si $ proba(\alpha) decrease \Rightarrow information(\alpha) increase $

**Exemple** : Scrable : W,E

Message : ensemble d'evenements

- l'informaiton véhiculée par un message dont les evenents sont indépendants les uns des autres = somme informations de chaques évenements pris indépendaments.
- l'informaiton véhiculée par un message dont les evenents sont dépendants les uns des autres < somme informations de chaques évenements pris indépendaments.

**Ex : scrable** Q,U

- cinq, coq, piqûre

$ \Rightarrow $ associations d'evenements : réduire la quantité d'information = compression de données

Information : $ I(\alpha_i) = f(p(\alpha_i)) $ $ 1 \leq i \leq n $ 

Evenements indépendants :
$$
I(\alpha_i + \alpha_j) = I(\alpha_i) + I(\alpha_j)
$$

$$
f(`p(\alpha_i) + p(\alpha_j)) = f(p(\alpha_i)) + f(p(\alpha_j))
$$

f : fonciton décroissante de $ p(\alpha_i) $
$$
I(\alpha_i) = \log_b({1 \over p(\alpha_i)}) = -\log_b(p(\alpha_i))
$$
b = 2 signaux binaires (bits)

<u>Sources binaires</u>
$$
P(0) = P(1) = { 1 \over 2 }
$$

$$
I(0) = I(1) = -\log_2({1 \over 2}) = 1 bit
$$

**Exemple image 8 niveaux de gris / pixels en 500x600 px**

$ N = 8^{{3.10^5}} $ images différentes
$$
I = \log_2 8^{{3.10^5}} \simeq 10^6 bits
$$
<u>Texte</u> : vocabulaire : 100 000 mots

message de 1000 mots parmi 100 000)
$$
I = 1000.\log_2 10^5 \simeq 2.10^4bits
$$

$$
I(\alpha_i) = -\log_2(p(\alpha_i))
$$

- signal aléatoire : alphabet de N symboles

$$
p(\alpha_i) = {1 \over N} \Rightarrow I(\alpha_i) = \log_2(N)
$$

- source markovienne

$$
I(\alpha_i) >> \log_2(N)
$$

$$
I(\alpha_j) << \log_2(N)
$$

**Exemple S = "9876543210000000000"**

$ P(9) = P(8) = ... = P(1) = {1 \over 20} $

$ I(9) = I(8) = ... = I(1) = 4.32bits $

$ P(0) = {11 \over 20} \rightarrow I(0) = 0.86bit $

répétition : $ \log_2 10 = 3.32bits $



Nombre de bits par symbole :

- Moins de bits possible pour coder "0"
- Codage à longueur variable : VLC



### 2) Entropie

Quantité sans dimension $ \mathscr{H}(m) = 3.17bits/symbole $

<u>Claude Shannon</u>

Mesurer l'incertitude sur la nature d'un message donné par rappor tà un message qui le précède

- Si aucune incertitude : Entropie nulle
- signal aléatoire : Entropie maximale

**Exemple image 256 couleur (8bits)

- Image noir : 0 bit/pixel
- Image avec du contenu structuré : $ 5bit/px \leq entropie \leq 7bit/px $
- Image aléatoire 8bit/pixel

Signal S de taille m
$$
\alpha_i \quad 0 \leq i \leq N \\
$$

$$
\mathscr{H}(S) = \sum_{i\:=\:0}^{N-1}p(\alpha_i)I(\alpha_i)
$$

$$
\mathscr{H}(S) = -\sum_{i\:=\:0}^{N-1}p(\alpha_i)\log_2p(\alpha_i)
$$

$$
\quad 0 \leq \mathscr{H} \leq \log_2N \\
$$

<u>Extension d'une source</u>

Source discrete sans mémoire (SDSM)

alphabet A: N symboles $ \alpha_i \quad 0 \leq i \leq N $

n evenements : blocs de k evenements (k : ordre markovien)

**Ex S = "010111100110111011001111100"**

N = 2, A = {0, 1}

Extension d'ordre 3 (k = 3) :

- B = {000, 001, 010, ..., 111}
- N' = 8 $ \quad \beta_j$ 

$$
S : p(\alpha_i) \\
S' = S^3 : p(\beta_j) = \pi.p(\alpha_i) \quad \mathscr{H}(S^k) = k.\mathscr{H}(S) \\
$$

Si dépendance des evenements: $ \mathscr{H}(S^k) < k.\mathscr{H}(S) $

**Exemple** image de 64x64 pixels.

- Source Markovienne d'ordre $ 64^2 = 4096 $  
- Blocs de 4x4 pixels : 16 pixels
- Ordre 16 : $ \mathscr{H}(Img^{16}) = 16.\mathscr{H}(Img) $

**Exemple** Passage de l'ordre 1 à l'ordre 2
$$
\mathscr{H}(S^2) = 2.\mathscr{H}(S)\\
S: \alpha_0 = 0\quad \alpha_1 = 1
$$
<u>Extension d'odre 2</u>
$$
S^2 : \beta_0 = 00 \quad \beta_1 = 01 \quad \beta_2 = 10 \quad \beta_3 = 11
$$

$$
\mathscr{H}(S^2) = -\sum_{j\:=\:0}^3p(\beta_j)\log_2p(\beta_j) \\
p(\alpha_0) = p_o \quad p(\alpha_1) = p_1
$$

$$
= -(p(\beta_0)\log_2p(\beta_0) + p(\beta_1)\log_2p(\beta_1) + p(\beta_2)\log_2p(\beta_2)) \\
= -(2p_0^2\log_2p_0 + 2p_0.p_1\log_2p_0.p_1 + 2p_1^2\log_2p_1) \\
= -2(p_0 + p_1)^2(p_0\log_2p_0 + p_1\log_2p_1) \\
= 2 \mathscr{H}(S)
$$

### 4) Mesure de distorsion

Image originale

(schéma)

$EQM = {1 \over N } \sum (p(i, j) - p'(i, j))^2 \quad $  N taille de l'image

$ SNR : {S \over N} \Rightarrow {S \over B} = 10.log_{10}{S \over N} dB $

Puissance max d'un pixel : valeure crète : $ P_{each} $

$ PSNR = 10\log_{10}{NDC_{image}^2 \over EQM} $

$ PSNR = 10\log_{10}{255^2 \over EQM} $

si psnr > 50 db => image très haute qualité (THQ)

30 < psnr <= 50 db => image de bonne qualité (BQ) 

20 < psnr <= 30 => moyenne qualité (MQ)

PSNR <= 20 dB => Médiocre

<u>Image couleur</u> : 3 images en NdG

RGB : 3 N pixels en NdB

















## TP1 - Palette

réduction des couleurs d'une image en une palette de couleurs pertinente

pas de jpeg compressé (PNG) ou format brut

codage 24 bit -> 8 bits 

taux de compression = 3

trouver plus proche couleur est remplacage dans palette

fichier text avec couleur de 0 a 255 puis valeur rgb (r g b)

2 couleur aux choix

ranger en fonciton de classe

puis reparcourir avec moyenen

au debut 2 couleur puis faire avec 256



k-means : 256 couleurs

-> 256 couleurs pseudo-aléatoires (initialisation)

ou

-> puis prendre couleur moyenne en 256

ou

-> decoupe image en grille de 32x32 et prendre les couleur grace a la moyenne



compte rendu



image ref

2 couluer init

image crée + itération (calculté distance entre de couleur) (distance euclidienne)

## TP2 - Espace couleur

Image RGB au format N x M $ \rightarrow $ 3 x M x N pixels

en sortie on veux 1.5 x M x N pixels (Taux de compression à deux)

ré-échantillonnage en prenant seulement 2 composantes

$ \tau = { tailleImageOriginale \over tailleImageCompressee } $

**1) Image compressé :**

$ R = {{M \times N} \over 4} $ avec ré-échantillonnage

$ G = G $

$ B = {{M \times N} \over 4} $ avec ré-échantillonnage



$ SNR = {S \over B} $

$ P_{ech}SNR = 20.log_{10} {255^2 \over EQM} $

$ EQM = {1 \over 3MN} \sum (p(i,j) -p'(i,j))^2 $



**2) RGB $ \rightarrow $ YCrCb :**

Même chose en sacrifiant Cr et Cb

**Conclusion** : cette compression marche mieux en YcrCb qu'en RGB



## Théorie de l'information

## Compression sans perte

## Compression avec perte

