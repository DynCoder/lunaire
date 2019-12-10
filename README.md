# Lunaire
Reading and modelling data gathered from a water rocket

## Why is it called Lunaire?
Because it comes from the french woord for moon. The -aire suffix means it is an adjective, making a lunaire-simulation a moon-simulation. This is because the program was made to track a water rocket, and rockets go up towards the moon.

## What does it do?
It takes a .lun file and models an object moving over those coordinates.

## .lun file format:
```
1000
x y z
0 0 0
0 0.2 0
0.1 0.4 0.2
0.3 0.5 0.6
0.5 0.6 1.1
```
```markdown
[timestep (amount of millisecond between data lines)]
[header (it really doesn't matter what you put on this line)]
[data (x coordinate, y coordinate and height)]
...
```
