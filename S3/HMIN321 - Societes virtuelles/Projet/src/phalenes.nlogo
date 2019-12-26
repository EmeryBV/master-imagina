breed[phalenes phalene]
breed[birds bird]
breed[arbres arbre]
breed[usines usine]

patches-own[pheromone]
phalenes-own[ctask free? lover-color state cpt-state male?]
birds-own[ctask cpt-state energy]

; tree, tree pine, flower, factory, plant, bird side egg, bug, pentagon, boat top, butterfly

to setup
  clear-all

  ask patches []

  create-phalenes nb-phalenes
  [
    setxy random-xcor random-ycor
    set size 4
    set free? false

    ifelse random-float 1 < %-white ; au départ les phalènes ne peuvent être que de 2 couleurs différentes
    [
      set color 7 ; blanc
    ][
      set color 4 ; noir
    ]

    set lover-color color

    ifelse random-float 1 < %-male
    [
      set male? true
    ][
      set male? false
    ]

    ifelse evolve?
    [
      set shape "egg"
      set state "egg"
      set cpt-state time-phalene-evolve
      set ctask "phalene-egg"
    ][
      set shape "butterfly"
      set state "butterfly"
      ifelse male?
      [
        set ctask "phalene-male-search-female"
      ][
        set ctask "phalene-female-search-tree"
      ]
    ]
  ]

  create-birds nb-birds
  [
    setxy random-xcor random-ycor
    set size 8
    set color red
    set shape "bird side"
    set ctask "bird-hunt"

    set cpt-state bird-life-time + random 50
  ]

  create-arbres nb-arbres
  [
    setxy random-xcor random-ycor
    set size 10
    set shape "tree-p"
    set color 8 - random-float 2
  ]

  ; Ainsi la fonction update arbres color n'est plus appelé à la création des usines mais à chaque tick pour modifier leur valeur (du coup prends du temps à chaque tick)
  create-usines nb-usines [
    setxy random-xcor random-ycor
    set size 15
    set color gray
    set shape "factory"
    update-arbres-color
  ]

  reset-ticks
end

to go
  if pheromones?
  [
    diffuse pheromone %-diffusion
    ask patches [colorate]
  ]

  ask phalenes [phalene-reflexes run ctask]
  ask birds    [bird-reflexes    run ctask]

  ask patches [evaporate]

  tick
end

;;;;;;;;;;;;; Utiles ;;;;;;;;;;;;;

to colorate
  set pcolor scale-color pink pheromone 1 (pheromone-max / 1.3)
end

to evaporate
  set pheromone pheromone - (pheromone * %-evaporation)
end

;;;;;;;;;;;;; Usine ;;;;;;;;;;;;;

to place-usine
  if mouse-down? [
    create-usines 1[
      setxy mouse-xcor mouse-ycor
      set size 15
      set color gray
      set shape "factory"
      update-arbres-color
    ]
    tick
  ]
end

to remove-usine
  if mouse-down? [
    let u min-one-of usines [distance patch mouse-xcor mouse-ycor]
    if u != nobody and [distance patch mouse-xcor mouse-ycor] of u <= 5
    [
      reset-arbres-color u
      ask u [die]
      tick
    ]
  ]
end

to update-arbres-color
  ask arbres [
    let u min-one-of usines [distance myself]
    if u != nobody [
      let d [distance myself] of u
      if d < max-polution-distance
      [
        set color color * (d / max-polution-distance)
        if color < 1 [ set color 1 ]
      ]
    ]
  ]
end

to reset-arbres-color [u]
  ask arbres [
    let d [distance myself] of u
    if d < max-polution-distance [
      set color color / (d / max-polution-distance)
      if color > 9.9 [ set color 9.9 ]
    ]
  ]
end


;;;;;;;;;;;;; Oiseau ;;;;;;;;;;;;;

to keep-vital-space
  let b min-one-of other breed in-radius bird-vital-space [distance myself]
  if b != nobody [
    ifelse patch-here = [patch-here] of b
    [
      fd bird-speed
    ][
      set heading towards b
      rt 180 + (20 - random 40)
      fd bird-speed
    ]
  ]
end

to bird-reflexes
  ifelse cpt-state <= 0 ; Mort de l'oiseau quand son compteur est inférieur à 0
  [
    die
  ][
    ifelse count phalenes / 10 > count birds ; Reproduction de l'oiseau quand la population totale est inferieur à 10% de celle des phalènes
    [
      hatch 1 [set cpt-state bird-life-time + random 50]
    ][
      let p (turtle-set (phalenes-here) (phalenes-on neighbors)) ; Mange la phalene sur le patch courant ou sur les voisins problème sino
      ifelse any? p
      [
        ask p [die]
      ][
        keep-vital-space ; Garde un espace vitale avec les autres oiseaux
      ]
    ]
    if bird-mortel? ; Le compteur ne baisse pas si l'oiseau n'est pas mortel
    [
      set cpt-state cpt-state - 1
    ]
  ]
end

to-report on-tree? ; detecte si le phalene est sur un arbre
  report any? arbres-on patch-here
end

to-report detected-on-tree?; detecte difficilement les phalene positionné sur les arbres de meme couleur
    let normalized-diff-color abs (([color] of one-of arbres-here / 10) - color / 10)
    report on-tree? and random-float 1 < %-detection-arbre * normalized-diff-color ;*
end

to-report detected-with-mean? ; detecte difficilement les phalene peut importe leur position si leur couleur est similaire à la moyenne generale de la couleur des arbres
  let normalized-diff-color abs ((mean [color] of arbres) / 10 - color / 10)
  report random-float 1 < normalized-diff-color
end

to bird-hunt

  let p nobody

  ifelse mean-detection? ; Change la méthode de détection
  [
    ; detecte les phalenes en fonction de la moyenne global de la couleur des arbres peut importe sa position
    set p min-one-of phalenes in-radius perception-birds with [detected-with-mean?] [distance myself]
  ][
    ; si le phalene est sur un arbre le phalène à une chance de ne pas se faire reperer selon ça couleur et celle de l'arbre sinon il se fait reperer quelque soit ça couleur
    set p min-one-of phalenes in-radius perception-birds with [not on-tree? or detected-on-tree?] [distance myself]
  ]

  ifelse p != nobody
  [
    set heading towards p
    fd bird-speed
  ][
    rt random 20
    lt random 20
    fd bird-speed
  ]
end

to add-bird
  create-birds 1
  [
    setxy random-xcor random-ycor
    set size 8
    set color red
    set shape "bird side"
    set ctask "bird-hunt"

    set cpt-state bird-life-time + random 50
  ]
end

to remove-bird
  ask one-of birds [die]
end

;;;;;;;;;;;;; Phalene Pré-Adulte ;;;;;;;;;;;;;

to phalene-wiggle
  rt random 20
  lt random 20
  fd phalene-speed
end

to phalene-reflexes ; Implemente l'évolution du papillon comme un reflexe
  ifelse not (state = "butterfly") and cpt-state <= 0
  [
    ifelse state = "egg"
    [
      set cpt-state time-phalene-evolve
      set shape "bug"
      set state "caterpillar"
      set ctask "phalene-caterpillar"
    ][
      ifelse state = "caterpillar"
      [
        set cpt-state time-phalene-evolve
        set shape "boat top"
        set state "chrysalis"
        set ctask "phalene-chrysalis"
      ][
        if state = "chrysalis"
        [
          set shape "butterfly"
          set state "butterfly"
          ifelse male?
          [
            set ctask "phalene-male-search-female"
          ][
            set ctask "phalene-female-search-tree"
          ]
        ]
      ]
    ]
  ][
    set cpt-state cpt-state - 1
  ]
end

to phalene-egg ; ne fait rien en oeuf
end

to phalene-caterpillar
  phalene-wiggle
end

to phalene-chrysalis ; ne fait rien en chrysalide
end

;;;;;;;;;;;;; Phalene Adulte ;;;;;;;;;;;;;

to add-phalene
  create-phalenes 1
  [
    setxy random-xcor random-ycor
    set size 4
    set free? false

    ifelse random-float 1 < %-white ; au départ les phalènes ne peuvent être que de 2 couleurs différentes
    [
      set color 7 ; blanc
    ][
      set color 4 ; noir
    ]

    set lover-color color

    ifelse random-float 1 < %-male
    [
      set male? true
    ][
      set male? false
    ]

    ifelse evolve?
    [
      set shape "egg"
      set state "egg"
      set cpt-state time-phalene-evolve
      set ctask "phalene-egg"
    ][
      set shape "butterfly"
      set state "butterfly"
      ifelse male?
      [
        set ctask "phalene-male-search-female"
      ][
        set ctask "phalene-female-search-tree"
      ]
    ]
  ]
end

to remove-phalene
  ask one-of phalenes [die]
end

;;;;;;;;;;;;; Femelle ;;;;;;;;;;;;;

to phalene-female-search-tree

  ifelse any? arbres-here
  [
    set ctask "phalene-female-waiting-male"
  ][
    let a min-one-of arbres in-radius perception-phalenes [distance myself]

    ifelse a != nobody
    [
      set heading towards a
      fd phalene-speed
    ][
      phalene-wiggle
    ]
  ]
end

to diffuse-pheromone
  set pheromone pheromone + pheromone-max
end

to phalene-female-waiting-male
  set free? true
  diffuse-pheromone
end

to phalene-female-go-away
  ifelse cpt-state = 0
  [
    phalene-female-lay-eggs
    die ; Mort à la ponte
  ][
    phalene-wiggle
  ]
end

to phalene-female-lay-eggs
  hatch-phalenes child-number
  [
    set size 4
    set free? false
    rt random 360

    ; Passage du gene couleur aux enfants

    ifelse random-float 1 < %-mutation
    [
      set color random-float 10 ; si mutation alors couleur aléatoire
    ][
      ifelse random-float 1 < 0.5 ; 1/2 chance d'avoir la couleur de la mère ou du père
      [
        set color [color] of myself ; Couleur de la mère
      ][
        set color [lover-color] of myself ; Couleur du père
      ]
    ]

    set lover-color color

    ifelse random-float 1 < %-male
    [
      set male? true
    ][
      set male? false
    ]

    ifelse evolve?
    [
      set shape "egg"
      set state "egg"
      set cpt-state time-phalene-evolve
      set ctask "phalene-egg"
    ][
      set shape "butterfly"
      set state "butterfly"
      ifelse male?
      [
        set ctask "phalene-male-search-female"
      ][
        set ctask "phalene-female-search-tree"
      ]
    ]
  ]
end

;;;;;;;;;;;;; Male ;;;;;;;;;;;;;

to-report free-female-here
  report one-of other phalenes-here with [not male? and free?]
end

to-report pheromone-here?
  report pheromone > 1
end

to follow-pheromone
  let p max-one-of neighbors [pheromone]
  set heading towards p
  fd phalene-speed
end

to phalene-male-search-female

  let f free-female-here

  ifelse f != nobody
  [
    ask f
    [
      set free? false
      set lover-color [color] of myself
      set cpt-state 20
      set ctask "phalene-female-go-away"
    ]
    die ; Mort à la reproduction
  ][
    ifelse pheromone-here?
    [
      follow-pheromone
    ][
      phalene-wiggle
    ]
  ]
end
@#$#@#$#@
GRAPHICS-WINDOW
467
24
1012
570
-1
-1
3.56
1
10
1
1
1
0
1
1
1
-75
75
-75
75
1
1
1
ticks
30.0

BUTTON
23
28
108
61
Setup
setup
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
113
28
194
61
Go
go
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SLIDER
22
66
194
99
nb-phalenes
nb-phalenes
0
200
40.0
1
1
NIL
HORIZONTAL

SLIDER
22
98
194
131
nb-birds
nb-birds
0
100
1.0
1
1
NIL
HORIZONTAL

SLIDER
240
46
412
79
%-male
%-male
0
1
0.5
0.01
1
NIL
HORIZONTAL

SLIDER
22
131
194
164
nb-arbres
nb-arbres
0
200
70.0
1
1
NIL
HORIZONTAL

SLIDER
240
176
412
209
perception-phalenes
perception-phalenes
0
100
30.0
1
1
NIL
HORIZONTAL

SLIDER
22
267
194
300
perception-birds
perception-birds
0
100
10.0
1
1
NIL
HORIZONTAL

SLIDER
240
354
412
387
%-diffusion
%-diffusion
0
1
1.0
0.01
1
NIL
HORIZONTAL

SLIDER
240
322
412
355
pheromone-max
pheromone-max
0
100
100.0
1
1
NIL
HORIZONTAL

PLOT
1041
15
1241
165
phalenes-population
NIL
NIL
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -4699768 true "" "plot count phalenes with [not male?]"
"pen-1" 1.0 0 -13345367 true "" "plot count phalenes with [male?]"

PLOT
1241
15
1441
165
birds-population
NIL
NIL
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -2674135 true "" "plot count birds"

SLIDER
240
386
412
419
%-evaporation
%-evaporation
0
1
0.05
0.01
1
NIL
HORIZONTAL

SLIDER
22
332
194
365
%-detection-arbre
%-detection-arbre
0
1
0.5
0.01
1
NIL
HORIZONTAL

SLIDER
240
249
412
282
time-phalene-evolve
time-phalene-evolve
0
100
10.0
1
1
NIL
HORIZONTAL

SLIDER
22
300
194
333
bird-vital-space
bird-vital-space
0
100
10.0
1
1
NIL
HORIZONTAL

SWITCH
240
216
412
249
evolve?
evolve?
1
1
-1000

SWITCH
240
289
412
322
pheromones?
pheromones?
0
1
-1000

SLIDER
240
425
412
458
child-number
child-number
0
100
4.0
1
1
NIL
HORIZONTAL

SLIDER
22
234
194
267
bird-speed
bird-speed
0
10
1.5
0.1
1
NIL
HORIZONTAL

SLIDER
240
144
412
177
phalene-speed
phalene-speed
0
10
1.0
0.1
1
NIL
HORIZONTAL

SLIDER
22
164
194
197
nb-usines
nb-usines
0
100
6.0
1
1
NIL
HORIZONTAL

SLIDER
240
564
411
597
max-polution-distance
max-polution-distance
0
1000
60.0
1
1
NIL
HORIZONTAL

BUTTON
240
492
411
525
NIL
place-usine
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

PLOT
1041
165
1241
315
phalenes-color-mean
NIL
NIL
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -5298144 true "" "plot mean [color] of phalenes"

PLOT
1241
165
1441
315
trees-color-mean
NIL
NIL
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -13840069 true "" "plot mean [color] of arbres"

SLIDER
240
111
412
144
%-mutation
%-mutation
0
1
0.1
0.01
1
NIL
HORIZONTAL

SLIDER
240
79
412
112
%-white
%-white
0
1
0.5
0.01
1
NIL
HORIZONTAL

SWITCH
24
371
195
404
bird-mortel?
bird-mortel?
0
1
-1000

SLIDER
24
404
195
437
bird-life-time
bird-life-time
0
1000
50.0
1
1
NIL
HORIZONTAL

TEXTBOX
293
19
443
37
Phalenes\n
12
0.0
1

TEXTBOX
88
209
238
227
Birds\n
12
0.0
1

TEXTBOX
300
468
450
486
Usines\n
12
0.0
1

BUTTON
25
592
196
625
NIL
add-bird
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
25
625
196
658
NIL
remove-bird\n
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SWITCH
25
445
195
478
mean-detection?
mean-detection?
0
1
-1000

PLOT
1041
315
1441
465
phalenes-most-representative-color
NIL
NIL
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"default" 1.0 0 -2674135 true "" "plot [color] of max-one-of phalenes [count phalenes with [color = [color] of myself]]"

PLOT
1041
465
1441
615
trees-most-representative-color
NIL
NIL
0.0
10.0
0.0
10.0
false
false
"" ""
PENS
"default" 10.0 1 -13840069 true "plot [color] of max-one-of arbres [count arbres with [color = [color] of myself]]" ""

BUTTON
240
528
411
561
NIL
remove-usine
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
25
514
196
547
NIL
add-phalene
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
25
547
196
580
NIL
remove-phalene\n
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

TEXTBOX
84
489
234
507
Manual\n
12
0.0
1

@#$#@#$#@
## WHAT IS IT?

Ce model essaie de montrer l'influence de l'envirronnement (polution, predateur, couleur des arbres) sur les phalènes du bouleau. Nous voulons montrer que si on a une forte pollution dans l'environnement cela influera sur la couleur des arbres qui aura un impact sur les phalène ayant un avantage selectif. L'avantage selectif ici sera bien sur la couleur des phalènes plus leur couleur est proche de celle des arbres environnant moins ils auront de chance de se faire manger par des prédateurs qui sont ici des oiseaux (en rouge).

## HOW IT WORKS

Au début de la simulation il n'y que deux couleur de phalène possible (4 ou 7)

La couleur des arbres est influencer par le nombre d'usine qu'il y a sur la carte

Le cycle des phalènes est le suivants :

La femelle essaie le plus rapidement possible de se diriger en sécurité dans un arbres.
Une fois dans cette arbres elle se met à émettre des phéromones pour attirer un male (champs de gradient rose).
Une fois fécondée elle s'éloigne de son arbre pour pondre des oeufs.

Le male quand à lui se déplace au hasard jusqu'a ce qu'il puisse suivre des traces de phéromomes pour féconder une femelle (il remonte le champ de gradient quand c'est le cas).

Les enfants sont générer à partir de la couleur des parents, il prendront soit la couleur de la mère soit celle du père ou alors il y a également une faible chance qu'ils subissent une mutation leurs faisants changer totalement de couleur (toujours en nuance de gris) (voir implementation de **phalene-female-lay-eggs**)

Les Nuances de gris en NetLogo sont représenté par un nombre de 0 à 9.9 un phalène à la naissance peut donc potientiellement avoir une chance d'avoir toute les variation possible de couleurs entre 0 et 9.9.

Le cycle des oiseaux est le suivants :

Un oiseau est crée si a chaque fois que la population d'oiseaux est inférieur à 1/10 de la population de phalène (cela permet de stabiliser les populations)

Une oiseaux se balade au hasard jusqu'a se qu'il detecte un phalène. Une fois la detection accomplie il essaiera de se rapprocher pour le manger.

La detection peut se faire de deux manières :

- Quand mean-detection? est sur on : l'oiseau detecte selon un pourcentage %-detection-arbre multiplié par la différence de la couleur du phalene avec la couleur moyenne des couleurs des arbres du monde (voir implementation dans **detected-with-mean?**)

**Remarque** : avec cette méthode les résultat l'émergence d'une nouvelle espèce dominante arrive vite mais la méthode n'est pas très réaliste.

- Quand mean-detection? est sur off :
	- L'oiseau verra à coup sur dans sont champ de vision les phalènes qui ne sont pas sur des arbres 
	- Si un phalène est percu mais est sur un arbre alors il y a une chance qu'il ne soit pas detecter qui est %-detection-arbre multiplié par la différence de couleur du phalène avec l'arbre sur lequel il se situe.

**Remarque** : cette méthode est plus réaliste que la précedente mais du coup les resultats mettent plus de temps à émerger même si ils émergent quand même final.

Dans tous les cas à la fin de la simulation on constate qu'une espèce de phalène plus adapté à la couleur des arbre environnant emerge.


## HOW TO USE IT

Appuyez sur le bouton "setup" pour mettre en place la simulation puis sur "go" pour faire vivre le monde.
Les paramètres sont régler de tel sorte que vous puissiez observer le résultat arriver de lui même mais libre à vous de changer les paramètres pour par exemple :

- ajouter / supprimer un oiseau dynamiquement pour controler l'évolution de la population des phalènes
- placer une usine avec la sourit sur la carte, grace au bouton "place-usine"
- changer le nombre d'enfants à la reproduction des phalènes etc...
- Le switch evolve? permet de visualiser les états différents du phalene (oeuf, chenille, chrysalide, papillon)
- On peut changer le mode de detection des oiseaux avec quelque chose de plus réaliste en mettant mean-detection? sur off

## THINGS TO NOTICE

Il y a deux couleur dans le plot phalènes population

- Rose : represente les phalènes femelles
- Bleu : represente les phalènes males

Au début de la simulation il n'y a que 2 couleurs différentes de phalènes (gris foncé / gris claire) cependant selon les paramètres de la simulation il est possible qu'une espèce de couleur différente emerge pouvant potentiellement devenir l'espèce dominante.

On peut observer qu'elle est l'espèce dominante avec  le plot "phalenes-most-representative-color"

Les plots liées au couleur vont de 0 a 10 car les valeurs de nuance de gris vont de 0 à 9.9 en netlogo

## THINGS TO TRY

- Essayez de demarrer la simulation avec un nombres différents d'usines pour voir qu'elle couleur de phalene emergera.
- Changer dynamiquement le nombre d'usine lors d'une simulation pour renverser la tendance de couleur chez les phalènes.
- Essayez de changer le nombre d'enfants générer par les phalènes
- Essayez de mettre la detection mean-detection? sur off pour avoir des resultats plus réaliste
@#$#@#$#@
default
true
0
Polygon -7500403 true true 150 5 40 250 150 205 260 250

airplane
true
0
Polygon -7500403 true true 150 0 135 15 120 60 120 105 15 165 15 195 120 180 135 240 105 270 120 285 150 270 180 285 210 270 165 240 180 180 285 195 285 165 180 105 180 60 165 15

arrow
true
0
Polygon -7500403 true true 150 0 0 150 105 150 105 293 195 293 195 150 300 150

beef
false
0
Polygon -7500403 true true 200 193 197 249 179 249 177 196 166 187 140 189 93 191 78 179 72 211 49 209 48 181 37 149 25 120 25 89 45 72 103 84 179 75 198 76 252 64 272 81 293 103 285 121 255 121 242 118 224 167
Polygon -7500403 true true 73 210 86 251 62 249 48 208
Polygon -7500403 true true 25 114 16 195 9 204 23 213 25 200 39 123
Line -7500403 true 240 75 255 45
Line -7500403 true 255 45 255 90
Polygon -7500403 true true 240 75 255 30 255 90

bird side
false
0
Polygon -7500403 true true 0 120 45 90 75 90 105 120 150 120 240 135 285 120 285 135 300 150 240 150 195 165 255 195 210 195 150 210 90 195 60 180 45 135
Circle -16777216 true false 38 98 14

boat top
true
0
Polygon -7500403 true true 150 1 137 18 123 46 110 87 102 150 106 208 114 258 123 286 175 287 183 258 193 209 198 150 191 87 178 46 163 17
Rectangle -16777216 false false 129 92 170 178
Rectangle -16777216 false false 120 63 180 93
Rectangle -7500403 true true 133 89 165 165
Polygon -11221820 true false 150 60 105 105 150 90 195 105
Polygon -16777216 false false 150 60 105 105 150 90 195 105
Rectangle -16777216 false false 135 178 165 262
Polygon -16777216 false false 134 262 144 286 158 286 166 262
Line -16777216 false 129 149 171 149
Line -16777216 false 166 262 188 252
Line -16777216 false 134 262 112 252
Line -16777216 false 150 2 149 62

box
false
0
Polygon -7500403 true true 150 285 285 225 285 75 150 135
Polygon -7500403 true true 150 135 15 75 150 15 285 75
Polygon -7500403 true true 15 75 15 225 150 285 150 135
Line -16777216 false 150 285 150 135
Line -16777216 false 150 135 15 75
Line -16777216 false 150 135 285 75

bug
true
0
Circle -7500403 true true 96 182 108
Circle -7500403 true true 110 127 80
Circle -7500403 true true 110 75 80
Line -7500403 true 150 100 80 30
Line -7500403 true 150 100 220 30

butterfly
true
0
Polygon -7500403 true true 150 165 209 199 225 225 225 255 195 270 165 255 150 240
Polygon -7500403 true true 150 165 89 198 75 225 75 255 105 270 135 255 150 240
Polygon -7500403 true true 139 148 100 105 55 90 25 90 10 105 10 135 25 180 40 195 85 194 139 163
Polygon -7500403 true true 162 150 200 105 245 90 275 90 290 105 290 135 275 180 260 195 215 195 162 165
Polygon -6459832 true false 150 255 135 225 120 150 135 120 150 105 165 120 180 150 165 225
Circle -6459832 true false 135 90 30
Line -6459832 false 150 105 195 60
Line -6459832 false 150 105 105 60

car
false
0
Polygon -7500403 true true 300 180 279 164 261 144 240 135 226 132 213 106 203 84 185 63 159 50 135 50 75 60 0 150 0 165 0 225 300 225 300 180
Circle -16777216 true false 180 180 90
Circle -16777216 true false 30 180 90
Polygon -16777216 true false 162 80 132 78 134 135 209 135 194 105 189 96 180 89
Circle -7500403 true true 47 195 58
Circle -7500403 true true 195 195 58

circle
false
0
Circle -7500403 true true 0 0 300

circle 2
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240

cylinder
false
0
Circle -7500403 true true 0 0 300

dot
false
0
Circle -7500403 true true 90 90 120

egg
false
0
Circle -7500403 true true 96 76 108
Circle -7500403 true true 72 104 156
Polygon -7500403 true true 221 149 195 101 106 99 80 148

face happy
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 255 90 239 62 213 47 191 67 179 90 203 109 218 150 225 192 218 210 203 227 181 251 194 236 217 212 240

face neutral
false
0
Circle -7500403 true true 8 7 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Rectangle -16777216 true false 60 195 240 225

face sad
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 168 90 184 62 210 47 232 67 244 90 220 109 205 150 198 192 205 210 220 227 242 251 229 236 206 212 183

factory
false
0
Rectangle -7500403 true true 76 194 285 270
Rectangle -7500403 true true 36 95 59 231
Rectangle -16777216 true false 90 210 270 240
Line -7500403 true 90 195 90 255
Line -7500403 true 120 195 120 255
Line -7500403 true 150 195 150 240
Line -7500403 true 180 195 180 255
Line -7500403 true 210 210 210 240
Line -7500403 true 240 210 240 240
Line -7500403 true 90 225 270 225
Circle -1 true false 37 73 32
Circle -1 true false 55 38 54
Circle -1 true false 96 21 42
Circle -1 true false 105 40 32
Circle -1 true false 129 19 42
Rectangle -7500403 true true 14 228 78 270

fish
false
0
Polygon -1 true false 44 131 21 87 15 86 0 120 15 150 0 180 13 214 20 212 45 166
Polygon -1 true false 135 195 119 235 95 218 76 210 46 204 60 165
Polygon -1 true false 75 45 83 77 71 103 86 114 166 78 135 60
Polygon -7500403 true true 30 136 151 77 226 81 280 119 292 146 292 160 287 170 270 195 195 210 151 212 30 166
Circle -16777216 true false 215 106 30

flag
false
0
Rectangle -7500403 true true 60 15 75 300
Polygon -7500403 true true 90 150 270 90 90 30
Line -7500403 true 75 135 90 135
Line -7500403 true 75 45 90 45

flower
false
0
Polygon -10899396 true false 135 120 165 165 180 210 180 240 150 300 165 300 195 240 195 195 165 135
Circle -7500403 true true 85 132 38
Circle -7500403 true true 130 147 38
Circle -7500403 true true 192 85 38
Circle -7500403 true true 85 40 38
Circle -7500403 true true 177 40 38
Circle -7500403 true true 177 132 38
Circle -7500403 true true 70 85 38
Circle -7500403 true true 130 25 38
Circle -7500403 true true 96 51 108
Circle -16777216 true false 113 68 74
Polygon -10899396 true false 189 233 219 188 249 173 279 188 234 218
Polygon -10899396 true false 180 255 150 210 105 210 75 240 135 240

house
false
0
Rectangle -7500403 true true 45 120 255 285
Rectangle -16777216 true false 120 210 180 285
Polygon -7500403 true true 15 120 150 15 285 120
Line -16777216 false 30 120 270 120

leaf
false
0
Polygon -7500403 true true 150 210 135 195 120 210 60 210 30 195 60 180 60 165 15 135 30 120 15 105 40 104 45 90 60 90 90 105 105 120 120 120 105 60 120 60 135 30 150 15 165 30 180 60 195 60 180 120 195 120 210 105 240 90 255 90 263 104 285 105 270 120 285 135 240 165 240 180 270 195 240 210 180 210 165 195
Polygon -7500403 true true 135 195 135 240 120 255 105 255 105 285 135 285 165 240 165 195

line
true
0
Line -7500403 true 150 0 150 300

line half
true
0
Line -7500403 true 150 0 150 150

pentagon
false
0
Polygon -7500403 true true 150 15 15 120 60 285 240 285 285 120

person
false
0
Circle -7500403 true true 110 5 80
Polygon -7500403 true true 105 90 120 195 90 285 105 300 135 300 150 225 165 300 195 300 210 285 180 195 195 90
Rectangle -7500403 true true 127 79 172 94
Polygon -7500403 true true 195 90 240 150 225 180 165 105
Polygon -7500403 true true 105 90 60 150 75 180 135 105

plant
false
0
Rectangle -7500403 true true 135 90 165 300
Polygon -7500403 true true 135 255 90 210 45 195 75 255 135 285
Polygon -7500403 true true 165 255 210 210 255 195 225 255 165 285
Polygon -7500403 true true 135 180 90 135 45 120 75 180 135 210
Polygon -7500403 true true 165 180 165 210 225 180 255 120 210 135
Polygon -7500403 true true 135 105 90 60 45 45 75 105 135 135
Polygon -7500403 true true 165 105 165 135 225 105 255 45 210 60
Polygon -7500403 true true 135 90 120 45 150 15 180 45 165 90

sheep
false
15
Circle -1 true true 203 65 88
Circle -1 true true 70 65 162
Circle -1 true true 150 105 120
Polygon -7500403 true false 218 120 240 165 255 165 278 120
Circle -7500403 true false 214 72 67
Rectangle -1 true true 164 223 179 298
Polygon -1 true true 45 285 30 285 30 240 15 195 45 210
Circle -1 true true 3 83 150
Rectangle -1 true true 65 221 80 296
Polygon -1 true true 195 285 210 285 210 240 240 210 195 210
Polygon -7500403 true false 276 85 285 105 302 99 294 83
Polygon -7500403 true false 219 85 210 105 193 99 201 83

square
false
0
Rectangle -7500403 true true 30 30 270 270

square 2
false
0
Rectangle -7500403 true true 30 30 270 270
Rectangle -16777216 true false 60 60 240 240

star
false
0
Polygon -7500403 true true 151 1 185 108 298 108 207 175 242 282 151 216 59 282 94 175 3 108 116 108

target
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240
Circle -7500403 true true 60 60 180
Circle -16777216 true false 90 90 120
Circle -7500403 true true 120 120 60

tree-p
false
3
Rectangle -6459832 true true 120 195 180 300
Circle -13840069 true false 118 3 94
Circle -13840069 true false 65 21 108
Circle -13840069 true false 116 41 127
Circle -13840069 true false 45 90 120
Circle -13840069 true false 104 74 152

triangle
false
0
Polygon -7500403 true true 150 30 15 255 285 255

triangle 2
false
0
Polygon -7500403 true true 150 30 15 255 285 255
Polygon -16777216 true false 151 99 225 223 75 224

truck
false
0
Rectangle -7500403 true true 4 45 195 187
Polygon -7500403 true true 296 193 296 150 259 134 244 104 208 104 207 194
Rectangle -1 true false 195 60 195 105
Polygon -16777216 true false 238 112 252 141 219 141 218 112
Circle -16777216 true false 234 174 42
Rectangle -7500403 true true 181 185 214 194
Circle -16777216 true false 144 174 42
Circle -16777216 true false 24 174 42
Circle -7500403 false true 24 174 42
Circle -7500403 false true 144 174 42
Circle -7500403 false true 234 174 42

turtle
true
0
Polygon -10899396 true false 215 204 240 233 246 254 228 266 215 252 193 210
Polygon -10899396 true false 195 90 225 75 245 75 260 89 269 108 261 124 240 105 225 105 210 105
Polygon -10899396 true false 105 90 75 75 55 75 40 89 31 108 39 124 60 105 75 105 90 105
Polygon -10899396 true false 132 85 134 64 107 51 108 17 150 2 192 18 192 52 169 65 172 87
Polygon -10899396 true false 85 204 60 233 54 254 72 266 85 252 107 210
Polygon -7500403 true true 119 75 179 75 209 101 224 135 220 225 175 261 128 261 81 224 74 135 88 99

wheel
false
0
Circle -7500403 true true 3 3 294
Circle -16777216 true false 30 30 240
Line -7500403 true 150 285 150 15
Line -7500403 true 15 150 285 150
Circle -7500403 true true 120 120 60
Line -7500403 true 216 40 79 269
Line -7500403 true 40 84 269 221
Line -7500403 true 40 216 269 79
Line -7500403 true 84 40 221 269

wolf
false
0
Polygon -16777216 true false 253 133 245 131 245 133
Polygon -7500403 true true 2 194 13 197 30 191 38 193 38 205 20 226 20 257 27 265 38 266 40 260 31 253 31 230 60 206 68 198 75 209 66 228 65 243 82 261 84 268 100 267 103 261 77 239 79 231 100 207 98 196 119 201 143 202 160 195 166 210 172 213 173 238 167 251 160 248 154 265 169 264 178 247 186 240 198 260 200 271 217 271 219 262 207 258 195 230 192 198 210 184 227 164 242 144 259 145 284 151 277 141 293 140 299 134 297 127 273 119 270 105
Polygon -7500403 true true -1 195 14 180 36 166 40 153 53 140 82 131 134 133 159 126 188 115 227 108 236 102 238 98 268 86 269 92 281 87 269 103 269 113

x
false
0
Polygon -7500403 true true 270 75 225 30 30 225 75 270
Polygon -7500403 true true 30 75 75 30 270 225 225 270
@#$#@#$#@
NetLogo 6.1.0
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
default
0.0
-0.2 0 0.0 1.0
0.0 1 1.0 0.0
0.2 0 0.0 1.0
link direction
true
0
Line -7500403 true 150 150 90 180
Line -7500403 true 150 150 210 180
@#$#@#$#@
0
@#$#@#$#@
