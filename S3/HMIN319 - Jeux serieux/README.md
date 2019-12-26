# HMIN319 - Jeux serieux

## Table des matières

[TOC]

## Informations

- [Planning](https://moodle.umontpellier.fr/course/view.php?id=1083)

### Examens

- **Note finale** : 30% Exam + 70% Projet

### Ressources

- [Cours et TDs-TPs](https://moodle.umontpellier.fr/course/view.php?id=1083)
- **Mails**
  - Damien Djaouti : damien@ludoscience.com

## Théorie de Game Design

### Modèle MDA

- **Mechanics** : Avoir une vision analytique pour élaborer un jeu
- **Dynamics** : Maintenir l’intérêt du joueur pour le jeu 
- **Aestetics** : Comprendre le ressenti du joueur

### Stéréotypes de joueurs

- Achievers
- Explorers
- Killers
- Socializers

### Flow

- Très haut niveau de concentration sur l'activité au détriment du reste
- Une perception du temps faussée
- La présence d'objectif à atteindre clair et précis.
- Une valorisation de l'effort intrinsèque à l'activité
- Un équilibre entre les capacités et l’objectif

## Jeu de cartes

- **Plateau de jeu**
  - Une matrice de cartes de valeur (1 à 10 pas de têtes sur le terrain)  constitue le terrain de jeu
  - Taille conseiller 4 de largeur et 7 de longueur (au choix des joueurs, on peut mélanger plusieurs paquets)
  - Les cartes non utilisées pour le terrain servent de pioche
- **Cartes spéciales**
  - **Valet** : On prend une carte dans la pioche, on la joue
  - **Dame** : Le joueur pioche 2 cartes | L'adversaire défausse 2 cartes | l'adversaire recule d'1 case
  - **Roi** : Permutation de 2 cartes sur le terrain
  - **As** : Représente une montagne infranchissable sur le terrain
  - **Joker** : Représente une carte de valeur 0 (déplacement assuré)
- **Règles du jeu**
  - **Général**
    - Le jeu se joue à 2 joueurs
    - Chaque joueur se place à l’extrémité du plateau du jeu en face à face
    - Le but du jeu est d'atteindre le coté de l'adversaire en se déplaçant de carte en carte sur le terrain
    - On ne peut pas avoir plus de 3 cartes en main
    - Les deux joueurs ne peuvent pas se trouver sur la même carte
  - **Déroulement de la partie**
    - Le joueur qui commence n'a pas de carte, l'autre pioche 1 carte
    - Le jeu se joue en tour par tour
    - On ne peut pas se déplacer en diagonale
    - Un tour est séparés en 3 phases :
      - **1ère phase** : Soit on pioche, soit on se déplace. Pour se déplacer on lance le dé 10 et on déplace son pion sur une case de valeur inférieure ou égale au dé. Si le dé a la même valeur que la case courante (Le pion est sur une case de valeur 3 et le dé a fait un 3) le joueur peut choisir de piocher 2 cartes au lieu de se déplacer
      - **2ème phase** : On peut jouer une carte. Cette action n'est pas obligatoire. Si c'est une carte spéciale, on effectue son action et on la place dans la défausse. Si c'est une carte normale (1 à 10) on la place sur le terrain où l'on veut (sauf sur la carte actuelle de l'autre joueur). La carte remplacée est placée dans la défausse.
      - **3ème phase** : On défausse des cartes jusqu'à avoir 3 cartes en main maximum.
  - **Fin de partie**
    - Quand un joueur atteint la dernière colonne. A partir de son prochain tour, lors d'un déplacement le dé doit avoir une valeur strictement supérieure à la carte actuelle.
    - Le premier qui atteint le coté de l'adversaire à gagné

## TIC-80 Game Jam

### Ressources

- [TIC-80 Wiki](https://github.com/nesbox/TIC-80/wiki)
- [TIC-80 Games]([TIC-80 Wiki](https://github.com/nesbox/TIC-80/wiki))

## Synthèse de texte : Le jeux peut-il être sérieux ?

### Jeu :

- decision à prendre avec limite de contrainte ou n'importe quoi
- incertitude margé
- conséquences faible mais pas trop non plus (pour conserver l'interet)

### Jeu $ \neq $ sérieux ? :

- jeu sérieux réside entre la frivolité et le sérieux
- Tension entre apprendre et jouer

## Conclusion

**Jeu** :

- s'immerger dans un monde avec ses propres règles (graphismes, gameplay)

**jeu sérieux** : 

- oblige de pousser la définition du terme jeu car notion de jeu dépend de la manière dont on perçoig l'activit

## Jeux

Les éléments d'un jeux sont les suivants

$$
\boxed{
\begin{array}{c}
	\fbox{Action}	\\
	\downarrow		\\
	\fbox{Etat}		\\
	\downarrow		\\
	\fbox{Règles}	\\
	\downarrow		\\
	\fbox{Agents}	\\
	\downarrow		\\
	\fbox{Buts}		\\
	\downarrow		\\
	\fbox{Facteurs d'echecs}
\end{array}
}
$$

## Projet

### Concept

- Public cible
- Chaque action du joueur -> feedback





```
<img src='https://g.gravizo.com/svg?
digraph G
{
  	node [shape = box];
  	
 	subgraph "Game design"
 	{
 		label = "Game design";
 		Gameplay -> Prototype;
 	}
 	
 	subgraph Prototypage
 	{
 		label = Prototypage;
 		color = blue;
 		a -> b -> c;
 	}
 	
 	Concept -> Gameplay;
	Concept -> a;
}
'/>
```

