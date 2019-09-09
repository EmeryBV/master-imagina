# Compilation

Utillisez make pour compiler

# Usage

./bin/droite <X0 Y0 X1 Y1> [BRESENHAM|REVEILLES|THICK] <WIDTH>

- <X0 Y0 X1 Y1> : les deux points de la ligne
- [BRESENHAM|REVEILLES|THICK] : choix de l'algo de dessin
- <WIDTH> : (Optionnel) largeur de la ligne

# Exemple

- Creation d'une ligne allant du point (10, 10) au point (30, 30) dans l'image out.pgm de taille (100, 100)

./bin/droite 10 10 30 30 bresenham > out.pgm

- Idem en rajoutant une seconde ligne avec y0 + 4 et y1 + 4

./bin/droite 10 10 30 30 bresenham 4 > out.pgm

- Creation d'une ligne d'épaisseur 10 dans out.pgm

./bin/droite 10 10 30 30 thick 10 > out.pgm
