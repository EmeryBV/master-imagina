globals [continue]

turtles-own [mygoal in-nodes out-nodes current plan]

patches-own [isgoal obstacle potential]

to setup
  clear-all
  ask patches [
    set isgoal 0
    set obstacle 0
    set potential 10000
  ]
  ask patch max-pxcor max-pycor [set-goal]
  create-agents
  ;; set one goal
  reset-ticks
end

to reset
  ask patches [
    set potential 100000;
    ifelse obstacle = 1 [
      set-obstacle
    ][
      ifelse (isgoal = 1) [
        set-goal
      ][
        set pcolor black
      ]
    ]
  ]
  ask turtles [die]
  create-agents
  reset-ticks
end

to move-to-next

end

to go
  ask turtles [
    set heading towards min-one-of neighbors [potential]
  ]
  repeat 5 [
    ask turtles [fd 0.2]
    display
  ]
  tick
end

to createGoals
  ask patch 0 5 [set-goal]
  ask patch -4 -8 [set-goal]
  repeat 3 [ask patch random-pxcor random-pycor [set-goal]]
end

to set-obstacle
  set obstacle 1
  if (isgoal = 1)[
    set isgoal 0
  ]
  set pcolor red
  set potential 100000
end

to set-goal
  set isgoal 1
  if (obstacle = 1)[
    set obstacle 0
  ]
  set pcolor yellow
  set potential 0
end

to erase-obstacle
  set pcolor black
  set obstacle 0
  set potential 1000000
end

to erase-potential
  ask patches [
    ifelse obstacle = 1 [
      set-obstacle
    ][
      ifelse (isgoal = 1) [
        set-goal
      ][
        set pcolor black
        set potential 1000000
      ]
    ]
  ]
  ask turtles [die]
end

to create-agents
  let xorg -15
  let yorg -15
  create-turtles number-turtles [
    set color orange
    set size 1.5
    ifelse (number-turtles = 1) [
      setxy xorg yorg
    ][
      setxy random-xcor random-ycor
    ]
    set in-nodes []
    set out-nodes []
    set mygoal min-one-of patches with [isgoal = 1] [distance self]
    set current init-current
    if (trace = true) [
      show (sentence "current node : " current "current goal " mygoal)
    ]
  ]
end

;;------------------------------------
;;
;; search machine
;;
;;------------------------------------

;; get the patch
to-report get-patch [node]
  report first node
end
;; get the value of the node
to-report get-val [node]
  report item 1 node
end

to-report get-cost [node]
  report item 2 node
end

to-report get-parent [node]
  report item 3 node
end

to search
  while [get-patch current != mygoal and [pcolor] of get-patch current != green] [
    search-step
  ]
  print "Success!!"
  draw-path current
end

to search-step
  let new-nodes generate
  set in-nodes merge-nodes new-nodes in-nodes
  if (trace = true) [print (sentence "size in-nodes : " length in-nodes)]
  set in-nodes sort-nodes in-nodes
  if (trace = true) [print (sentence "sorted in-nodes: " length in-nodes ", " in-nodes)]
  let proposed first in-nodes
  if (proposed = [])[
    stop
  ]
  set current proposed
  set in-nodes bf in-nodes
  set out-nodes (fput (get-patch current) out-nodes) ;; we only put patches in the out-nodes list
  if (trace = true) [show (sentence "current node : " current)]
  ask (get-patch current) [if (pcolor != green)[set pcolor blue]]
end

to-report generate
  let aset nobody
  set aset ([neighbors] of (get-patch current)) with [obstacle = 0]
  ifelse (any? aset) [
    ask aset  [
      if (pcolor = black)[
        set pcolor white
      ]
    ]
    let lst1 [self] of aset ;; transformer en listes...
                            ;; show (sentence "aset : " lst1)
    set lst1 (remove-nodes lst1 out-nodes) ;; delete nodes which have already been visited
    let result map [ x -> make-node x current ] lst1 ;; make a list of nodes [patch value]
    report result
  ][
    show (sentence "blocked ;-) ")
    report []
  ]
end


;; NOTE: DEPENDS ON THE ALGORITHM
to-report init-current
  ifelse Algorithm = "Dijkstra" [
    report (make-node1 patch-here 0 0 [])
  ][
    report (make-node1 patch-here distance-to-goal patch-here 0 [])
  ]
end

;; NOTE: DEPENDS ON THE ALGORITHM
to-report make-node [p n]
  let r 0
  ifelse Algorithm = "Dijkstra" [
    set r (list p (1 + (get-val n)) 0 n)
  ][
    let g 1 + get-cost n
    let h distance-to-goal p
    ;; Note: holds the cost and the parent.
    ;; Used by A* algorithm
    set r (make-node1 p h g n)
  ]
  report r
end

to-report make-node1 [p h c n]
  report (list p h c n)
end

to-report cost [node]
  ifelse (Algorithm = "Astar") [
   ;  report (get-val node) + (get-cost node)
     report (get-cost node)
  ][
     report (get-val node)
  ]
end

to-report distance-to-goal [p]
  let r 0
  set r [distance p] of mygoal
 ;; show (sentence "distance from " p " to " goal " = " r)
  report r
end



to-report sort-nodes [lst-nodes]
   ; let lst sort-by [(cost ?1) < (cost ?2)] lst-nodes
  let lst []
  ifelse (Algorithm != "Astar")[
    set lst sort-by [[x y] -> (get-val x) < (get-val y)] lst-nodes
  ][
    set lst sort-by [[x y] -> (get-val x)+ weight * (cost x) < (get-val y)+ weight * (cost y)] lst-nodes ;; take the weight of the cost g (and not only the heuristic h*) into account
  ]
  if (lst = []) [
    (show "pas de solution !!" )
    report []
  ]
  report lst
end


;; crÃ©e une liste avec le min des couts...
to-report merge-nodes [lst-new lst-ref]
  let res []
  ;set lst-ref sort-by [(cost ?1) < (cost ?2)] lst-ref
  let lst map get-patch lst-ref
  while [lst-new != []] [
    let node (first lst-new)
    let pos position (get-patch node) lst
    ifelse (pos = false)[
      set res (fput node res)
    ][
      let v 0
      let g 0
      let p current
      ifelse ((cost node) < (cost (item pos lst-ref))) [
        set v get-val node
        set g get-cost node
        set p current
      ][
       set v get-val (item pos lst-ref)
        set g get-cost (item pos lst-ref)
        if (Algorithm != "Best-first") [
           set p get-parent (item pos lst-ref)
        ]
      ]
      ;; reconstruct the node
      set res (fput (make-node1 (get-patch node) v g p) res)
      ;; suppress the node from old list
      set lst-ref remove-item pos lst-ref
      set lst map get-patch lst-ref
    ]
    set lst-new (bf lst-new)
  ]
  set res (sentence res lst-ref)
  report res
end

to-report remove-nodes [lst ref-lst]
  let res []
  while [lst != []] [
    if not member? (first lst) ref-lst [
      set res (fput (first lst) res)
    ]
    set lst (bf lst)
  ]
  report res
end

to draw-path [node]
  let current-potential [potential] of get-patch node
  while [node != []] [
    ask get-patch node [
      set pcolor green
      set potential current-potential
    ]
    set node get-parent node
    set current-potential (current-potential + 1)
  ]
end

;;------------------------------------
;;
;;    Turtles
;;
;;-------------------------------------

to search-turtles
  ask-concurrent turtles [search]
  tick
end

to step-turtles
  ask turtles [search-step]
end


;;----------------------------------------
;;
;;   drawing
;;
;;----------------------------------------

to draw-goals
  if mouse-down? [
    ask patches [
      if ((abs (pxcor - mouse-xcor)) < 1) and ((abs (pycor - mouse-ycor)) < 1) [
        set-goal
      ]
    ]
  ]
  display
end

to draw-obstacles
  if mouse-down? [
    ask patches [
      if ((abs (pxcor - mouse-xcor)) < 1) and ((abs (pycor - mouse-ycor)) < 1)[
        set-obstacle
      ]
    ]
  ]
  display
end

to eraser
  if mouse-down? [
   ask patches [
      if ((abs (pxcor - mouse-xcor)) < 1) and ((abs (pycor - mouse-ycor)) < 1) [
        erase-obstacle
      ]
    ]
  ]
  display
end
@#$#@#$#@
GRAPHICS-WINDOW
262
10
762
511
-1
-1
12.0
1
12
1
1
1
0
0
0
1
-20
20
-20
20
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
6
54
126
87
NIL
search-turtles
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
2
249
111
282
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
3
296
113
329
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

SLIDER
4
145
144
178
number-turtles
number-turtles
1
10
6.0
1
1
NIL
HORIZONTAL

BUTTON
132
53
237
86
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
121
250
221
283
Draw goals
draw-goals
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
133
12
196
45
NIL
reset
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

CHOOSER
7
190
145
235
Algorithm
Algorithm
"Best-first" "Dijkstra" "Astar"
2

SWITCH
150
190
240
223
Trace
Trace
1
1
-1000

SLIDER
6
347
178
380
weight
weight
0
1
0.75
0.01
1
NIL
HORIZONTAL

BUTTON
24
98
87
131
NIL
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
15
411
187
444
potential-level
potential-level
0
100
100.0
1
1
NIL
HORIZONTAL

BUTTON
122
99
209
132
go-step
go
NIL
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

A Generic search algorithm for A* (Astar), Dijkstra and 'best first' in a tile world (patch world as used in some games).

## HOW IT WORKS
Use a generic algorithm where only a few variations makes the difference between A*, Dijskstra and best first.

## HOW TO USE IT
Type 'reset' to start a new search without removing obstacles.. Setup clears all
Draw obstacles (and click again to stop the process)
Choose an algorithm (default is A*)
Click on 'search-turtles' to get the whole search or 'step-turtle' to move forward one step at a time

- Red patches are obstacles
- Yellow patches are the goals
- Blue patches are the one that have been visited by the engine
- White patches are generated through the algorithm (neighbors of visited nodes)
- Green patches form the path found by the algorithm

## THINGS TO NOTICE

Notice the difference between those three algorithms. The difference lies in the make-node function, and in the evaluation function.
*Note that the turtle does not move!* Exploration is done via the engine, but not through the behavior of a turtle. This is a "path planning" model and not a "space exploration" one.

## THINGS TO TRY

Draw obstacles and goals as you wish... See how it works step by step...

## CREDITS AND REFERENCES

(C) Jacques Ferber - wwww.lirmm.fr/~ferber   - 2014
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
NetLogo 6.0.4
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
