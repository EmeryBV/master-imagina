# HMIN235 - Algorithmes geometriques et geometrie discrete

## Sommaire [↺](#sommaire-)

<!-- MarkdownTOC autolink=true -->

- [Informations ↺](#informations-%E2%86%BA)
	- [Examens ↺](#examens-%E2%86%BA)
	- [Ressources ↺](#ressources-%E2%86%BA)
- [Algorithmes géométriques](#algorithmes-g%C3%A9om%C3%A9triques)
	- [Applications linéaires](#applications-lin%C3%A9aires)
- [TD-TP-4 Exercice 2](#td-tp-4-exercice-2)
	- [Applications affines](#applications-affines)
	- [Transformations bifocales](#transformations-bifocales)
	- [Transformations fisheye](#transformations-fisheye)
- [Géometrie discrete](#g%C3%A9ometrie-discrete)
- [Trioker](#trioker)
- [Géométrie discrete](#g%C3%A9om%C3%A9trie-discrete)
	- [Notion de distance](#notion-de-distance)
		- [Axiome des distances](#axiome-des-distances)
		- [Distance Hausdorff](#distance-hausdorff)
		- [Norme](#norme)
	- [TD - Les masques de Chamfrein](#td---les-masques-de-chamfrein)
		- [Exercice 1](#exercice-1)
		- [Exercice 2](#exercice-2)
		- [Exercice 3](#exercice-3)
		- [Exercice 4](#exercice-4)
		- [Exercice 5](#exercice-5)

<!-- /MarkdownTOC -->


## Informations [↺](#sommaire-)

- Anciennemnt : HMIN214

### Examens [↺](#sommaire-)

- Note final : max(50% CC + 50% Exam, Exam)
	- Examen
	- Controle
		- participation
		- rendu de l'exercice

### Ressources [↺](#sommaire-)

- [Cours et TDs-TPs 1](http://www.lirmm.fr/~mountaz/Ens/AlgoGeo/)
- [Cours et TDs-TPs 2](https://www.lirmm.fr/~bessy/AlgoGeo/accueil.html)

## Algorithmes géométriques

**Géométrie** : science de touts les types d'espaces

### Applications linéaires

s'applique aux éléments d'un espaces vectoriel

**Endomorphisme** : l'espace de départ et le même que celui d'arrivé

- Homothétie (de rapport k ∈ R), de centre O
- Rotation, d'angle α, de centre O
- Symétrie par rapport à une droite, passant par O
- Homothétie (de rapport k ∈ R), de centre O
- Homothétie (de rapport k ∈ R), de centre O

toute application linéaire à une représenation par une matrice

# TD-TP-4 Exercice 2

- image 2 : on utilise un simple rotation
- image 3 :

### Applications affines

### Transformations bifocales

### Transformations fisheye

## Géometrie discrete

## Trioker

- Point2 : (double x, y)
	- translate(Vector2)
	- rotate(Point2, double angle)

- Vector2 : (double x, y)
	- add(Vector2)
	- soustract(Vector2)
	- multiply(double)
	- divide(double)
	- equals(Vector2)
	- normalize
	- double angle()
	- double lengthSquared()
	- double length()
	- double distanceSquared()
	- double distance()

- Triangle : (3 Point2)
	- center()
	- contains(Point2)
	- translate(Vecteur2)
	- rotate(double)

- Trioker : (Triangle && String label)
	- rotateLeft()
	- rotateRight()
	- invert()

## Géométrie discrete

### Notion de distance

$d_2(x, y) = \sqrt{ (x_2 - x_1)² + (y_2 - y_1)² }$

#### Axiome des distances

1. Positive $∀ p, q ∈ E, d(p, q) ≥ 0$
2. Definie $∀ p, q ∈ E, d(p, q) = 0 ⇔ p = q$
3. Symétrique $∀ p, q ∈ E, d(p, q) = d(q, p)$
4. Triangulaire $∀ p, q ∈ E, d(p, q) ≤ d(p, r) + d(r, q)$

#### Distance Hausdorff

$d(p, X) = min(d(p, q) : q ∈ X))$

$Hd(X, Y) = max(max(d(p, Y) : p ∈ X), max(d(p, X) : p ∈ Y))$

#### Norme

1. Positive : $∀ x ∈ E, g(x) ≥ 0$
2. Definie : $∀ x ∈ E, g(x) = 0 ⇔ x = q$
3. Triangulaire : $∀ x, y ∈ E, g(x, y) ≤ g(x) + g(y)$
4. Homogene : $∀ x ∈ E, λ ∈ K, g(λ.x) = |λ|.g(x)$
5. Invariance par translation : $∀ p, q, r ∈ E d(p + r, q + r) = d(p, q)$
6. Homogénéité : $∀ p, q ∈ E, λ ∈ k d(λ.p, λq) = |λ|.d(p, q)$

Soit $g : E → F$ une norme sur K

Alors l'application $D : E × E → F$

$(p, q) ↦ g(q - p)$  est une distance

### [TD - Les masques de Chamfrein](TDs/TD%20-%20Distance%20de%20Chamfrein.pdf)

#### Exercice 1

1. Distance de Manhattan
2. Distance de l'échiquier $2^2$
3. 

#### Exercice 2

1. L'inegalité triangulaire est la propriété qui nous permet cela

2. $\begin{vmatrix}2 & 1 & 2\\1 & 0 & 1\\2 & 1 & 4\end{vmatrix}$ et $\begin{vmatrix}14 & 11 & 10 & 11 & 14\\11 & 7 & 5 & 7 & 11\\10 & 5 & 0 & 5 & 10\\11 & 7 & 5 & 7 & 11\\14 & 11 & 10 & 11 & 14\end{vmatrix}$

#### Exercice 3



#### Exercice 4



#### Exercice 5

