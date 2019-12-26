## 	HMIN322 - Compte Rendu du TP2

Réalisé par **Odorico Thibault**

## Tables des matières

[TOC]

## Image de référence

![baboon](.README.assets/baboon.png)

## Compression (RGB) sur les composantes Bleu et Rouge

![rgb](.README.assets/rgb.png)

En zoomant dans l'image on peut remarquer que les pixels verts sont beaucoup plus présent que les autres pixels, Le calcul du PSNR nous donne un resultat de 22.8376dB ce qui indique que l'image resultante est de qualité moyenne par rapport à l'originale

## Compression (YCbCr) sur les Composantes Cr et Cb

![YCbCr](.README.assets/YCbCr.png)

Je pense que le resultat n'est pas celui que j'aurais du obtenir, l'image ci-dessus à un PSNR de 14.5396 ce qui correspond à une qualité d'image médiocre, dans mon cas la compression RGB à été plus efficace mais je pense qu'il faudrait revoir l'algorithme de compression de YCbCr.