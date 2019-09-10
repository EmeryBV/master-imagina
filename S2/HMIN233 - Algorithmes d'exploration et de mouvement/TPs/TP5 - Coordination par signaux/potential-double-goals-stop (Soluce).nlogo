globals [continue]

patches-own [
  goal
  obstacle
  potential1
  potential2
  repulsive
]

turtles-own [
  satisfaction
  empathie
  randcolor
]

to setup
  clear-all
  ask patches [
    set goal 0
    set obstacle 0
    set potential1 0
    set potential2 0
    set repulsive 0
  ]
  createObstacles
  ;; createGoals

  create-agents

  reset-ticks
end


to createObstacles
  ask patch 3 0 [set-obstacle]
  ask patch 3 1 [set-obstacle]
  ask patch 3 2 [set-obstacle]
  ask patch 2 2 [set-obstacle]
  ask patch 1 2 [set-obstacle]
  ask patch 0 2 [set-obstacle]
  ask patch -1 2 [set-obstacle]
  ask patch -2 2 [set-obstacle]
  ask patch -3 2 [set-obstacle]
  ask patch -3 1 [set-obstacle]
  ask patch -3 0 [set-obstacle]
  ask patch -3 -1 [set-obstacle]
  ask patch -3 -2 [set-obstacle]
  ask patch -3 -3 [set-obstacle]
  ask patch -3 -4 [set-obstacle]
  ask patch -3 -5 [set-obstacle]
  ask patch -3 -6 [set-obstacle]
  ask patch -3 -7 [set-obstacle]
  ask patch -3 -8 [set-obstacle]
  ask patch -3 -9 [set-obstacle]
  ask patch -4 -9 [set-obstacle]
  ask patch -5 -9 [set-obstacle]
end

to createGoals
  ask patch 0 5 [set-goal1]
  ask patch -4 -8 [set-goal2]
  repeat 3 [ask patch random-pxcor random-pycor [set-goal1]]
end


to set-obstacle
  set obstacle 1
  set pcolor red
  set repulsive (- potential-level)
end

to set-goal1
  set goal 1
   set pcolor yellow
   set potential1 potential-level
end

to set-goal2
  set goal 1
   set pcolor blue
   set potential2 potential-level
end

to erase-obstacle
  set pcolor black set obstacle 0
  set potential1 0
  set potential2 0
end

to erase-potential
  ask patches [
    ifelse obstacle = 1
    [set-obstacle]
    [ifelse (goal = 1)
      [ifelse (pcolor = yellow)[set-goal1][set-goal2]]
      [set pcolor black set potential1 0 set potential2 0 set repulsive 0]
    ]
  ]
  ask turtles [die]
end

to create-agents
  create-turtles number-turtles [
   ; set color orange
    set randcolor random 2
      ifelse randcolor = 0
      [set color orange]
      [set color turquoise]

    set size 1.5
    setxy random-xcor random-ycor
  ]
end

to compute-field
  compute-potential
  if (repulsive-field?) [compute-repulsive]
end

to compute-field1
  compute-potential1
  compute-potential2
  if (repulsive-field?) [compute-repulsive1]
end

 to compute-potential
  set continue true
  while [continue][
    set continue false
    compute-potential1
    tick
  ]
  set continue true
  while [continue][
    set continue false
    compute-potential2
    tick
  ]
end

  to compute-potential1
   ask patches [
      if (goal = 0) and (obstacle = 0)[
        let old-pot1 potential1
        let pot1 max-one-of neighbors4 [potential1]
        let p1 [potential1] of pot1 - 1

        if (potential1 <= p1) and (p1 != old-pot1) [
          set potential1 p1
          ifelse potential1 < 0
             [set potential1 0]
             [set continue true]
        ]
        draw-field
      ]
  ]
end

  to compute-potential2
   ask patches [
      if (goal = 0) and (obstacle = 0)[
        let old-pot2 potential2
        let pot2 max-one-of neighbors4 [potential2]
        let p2 [potential2] of pot2 - 1

        if (potential2 <= p2) and (p2 != old-pot2) [
          set potential2 p2
          ifelse potential2 < 0
             [set potential2 0]
             [set continue true]
        ]
        draw-field
      ]
  ]
end



to compute-repulsive
  set continue true
  while [continue][
    set continue false
    compute-repulsive1
    tick
  ]
end

  to compute-repulsive1
   ask patches [
      if (goal = 0) and (goal = 0)[
        let old-pot repulsive
        let pot min-one-of neighbors4 [repulsive]
        let p [repulsive] of pot + repulsive-increment
        if (repulsive >= p) and (p != old-pot) [
          set repulsive p
          ifelse repulsive > 0
             [set repulsive 0]
             [set continue true]
        ]
        draw-field
    ]
  ]
end

to draw-field
  if (goal = 0) and (obstacle = 0) [
  ifelse ((- repulsive) > potential1) and ((- repulsive) > potential2)[
              set pcolor scale-color orange repulsive ((- potential-level) * 1.2) 0
  ][
   ifelse (potential1 > potential2)[
        set pcolor scale-color green potential1 0 (potential-level * 1.2)
     ][
        set pcolor scale-color blue potential2 0 (potential-level * 1.2)
     ]
  ]
]
end

;;------------------------------------
;;
;;    Turtles
;;
;;-------------------------------------

to go-turtles
  ask turtles [go-goal]
  tick
end

to step-turtles
  ask turtles [go-goal]
end

to go-goal
  ifelse color = orange
  [ go-goal1]
  [go-goal2]

end

to go-goal1
  if goal != 1 [
      let way neighbors with [obstacle = 0]
      let path max-one-of way [potential1 + repulsive]
      if path != nobody [
        set heading towards path
      ]
      rt random 30
      lt random 30
    ;; stop if there is a turtle on the way
    let p patch-ahead 1
    if (p != nobody) and (not any? other turtles-on p) [
      fd 1
    ]
  ]
end

to go-goal2
  if goal != 1 [
      let way neighbors with [obstacle = 0]
      let path max-one-of way [potential2 + repulsive]
      if path != nobody [
        set heading towards path
      ]
      rt random 30
      lt random 30
    ;; stop if there is a turtle on the way
    let p patch-ahead 1
    if (p != nobody) and (not any? other turtles-on p) [
      fd 1
    ]
  ]
end

;;----------------------------------------
;;
;;   drawing
;;
;;----------------------------------------

to draw-goals1
  if mouse-down?
  [ ask patches
    [ if ((abs (pxcor - mouse-xcor)) < 1) and ((abs (pycor - mouse-ycor)) < 1)
      [ set-goal1]]]
  display
end

to draw-goals2
  if mouse-down?
  [ ask patches
    [ if ((abs (pxcor - mouse-xcor)) < 1) and ((abs (pycor - mouse-ycor)) < 1)
      [ set-goal2]]]
  display
end



to draw-obstacles
  if mouse-down?
  [ ask patches
    [ if ((abs (pxcor - mouse-xcor)) < 1) and ((abs (pycor - mouse-ycor)) < 1)
      [ set-obstacle]]]
  display
end

to eraser
  if mouse-down?
  [ ask patches
    [ if ((abs (pxcor - mouse-xcor)) < 1) and ((abs (pycor - mouse-ycor)) < 1)
      [ erase-obstacle]]]
  display
end
@#$#@#$#@
GRAPHICS-WINDOW
238
16
894
673
-1
-1
8.0
1
10
1
1
1
0
0
0
1
-40
40
-40
40
0
0
1
ticks
30.0

BUTTON
8
13
104
46
NIL
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
5
189
100
222
NIL
go-turtles
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
4
344
113
377
Draw Obstacles
draw-obstacles
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
5
391
115
424
Erase obstacles
eraser
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
6
57
105
90
potential field
compute-field
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SLIDER
4
435
217
468
potential-level
potential-level
0
100
51.0
1
1
NIL
HORIZONTAL

BUTTON
112
57
210
90
step potential
compute-field1
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
112
12
209
45
Erase potential
erase-potential
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SLIDER
4
145
176
178
number-turtles
number-turtles
0
200
42.0
1
1
NIL
HORIZONTAL

BUTTON
108
188
213
221
NIL
step-turtles
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
123
345
237
378
Draw goals-1
draw-goals1
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
7
102
106
135
Create agents
create-agents
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SLIDER
5
472
219
505
repulsive-increment
repulsive-increment
0
20
11.5
0.5
1
NIL
HORIZONTAL

SWITCH
3
229
155
262
repulsive-field?
repulsive-field?
1
1
-1000

BUTTON
127
396
237
429
NIL
draw-goals2
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

@#$#@#$#@
## WHAT IS IT?

A flooding algorithm for creating potential fields with obstacles

## HOW IT WORKS

(what rules the agents use to create the overall behavior of the model)
The 'step potential' button

## HOW TO USE IT

draw goals and then use '

## THINGS TO NOTICE

(suggested things for the user to notice while running the model)

## THINGS TO TRY

(suggested things for the user to try to do (move sliders, switches, etc.) with the model)
Draw obstacles that allow for only a part
Change the value of the potential field. See the behavior of agents when there is no field (it's black). They do not go towards the goals.

## EXTENDING THE MODEL

(suggested things to add or change in the Code tab to make the model more complicated, detailed, accurate, etc.)

## NETLOGO FEATURES

(interesting or unusual features of NetLogo that the model uses, particularly in the Code tab; or where workarounds were needed for missing features)

## RELATED MODELS

(models in the NetLogo Models Library and elsewhere which are of related interest)

## CREDITS AND REFERENCES

(C) J. Ferber 2014 
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
Polygon -16777216 true false 150 255 135 225 120 150 135 120 150 105 165 120 180 150 165 225
Circle -16777216 true false 135 90 30
Line -16777216 false 150 105 195 60
Line -16777216 false 150 105 105 60

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

cow
false
0
Polygon -7500403 true true 200 193 197 249 179 249 177 196 166 187 140 189 93 191 78 179 72 211 49 209 48 181 37 149 25 120 25 89 45 72 103 84 179 75 198 76 252 64 272 81 293 103 285 121 255 121 242 118 224 167
Polygon -7500403 true true 73 210 86 251 62 249 48 208
Polygon -7500403 true true 25 114 16 195 9 204 23 213 25 200 39 123

cylinder
false
0
Circle -7500403 true true 0 0 300

dot
false
0
Circle -7500403 true true 90 90 120

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
0
Rectangle -7500403 true true 151 225 180 285
Rectangle -7500403 true true 47 225 75 285
Rectangle -7500403 true true 15 75 210 225
Circle -7500403 true true 135 75 150
Circle -16777216 true false 165 76 116

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

tree
false
0
Circle -7500403 true true 118 3 94
Rectangle -6459832 true false 120 195 180 300
Circle -7500403 true true 65 21 108
Circle -7500403 true true 116 41 127
Circle -7500403 true true 45 90 120
Circle -7500403 true true 104 74 152

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
Polygon -7500403 true true 135 285 195 285 270 90 30 90 105 285
Polygon -7500403 true true 270 90 225 15 180 90
Polygon -7500403 true true 30 90 75 15 120 90
Circle -1 true false 183 138 24
Circle -1 true false 93 138 24

x
false
0
Polygon -7500403 true true 270 75 225 30 30 225 75 270
Polygon -7500403 true true 30 75 75 30 270 225 225 270
@#$#@#$#@
NetLogo 6.0.2
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
