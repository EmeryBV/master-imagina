#ifndef IMAGE_PGM_PPM_H
#define IMAGE_PGM_PPM_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char OCTET;

#define allocation_tableau(nom, type, nombre)                                 \
  if ((nom = (type *)calloc(nombre, sizeof(type))) == NULL) {                 \
    printf("\n Allocation dynamique impossible pour un pointeur-tableau \n"); \
    exit(EXIT_FAILURE);                                                       \
  }

void ignorer_commentaires(FILE *f);

void ecrire_image_ppm(const char nom_image[], const OCTET *pt_image, int nb_lignes, int nb_colonnes);
void ecrire_image_pgm(const char nom_image[], const OCTET *pt_image, int nb_lignes, int nb_colonnes);

void lire_nb_lignes_colonnes_image_ppm(const char nom_image[], int *nb_lignes, int *nb_colonnes);
void lire_nb_lignes_colonnes_image_pgm(const char nom_image[], int *nb_lignes, int *nb_colonnes);

void lire_image_ppm(const char nom_image[], OCTET *pt_image, int taille_image);
void lire_image_pgm(const char nom_image[], OCTET *pt_image, int taille_image);

void planR(OCTET *pt_image, const OCTET *src, int taille_image);
void planV(OCTET *pt_image, const OCTET *src, int taille_image);
void planB(OCTET *pt_image, const OCTET *src, int taille_image);

#endif 
