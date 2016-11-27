# GDVoronoi

Implementation of Fortune's Algorithm for Godot Engine. Adapted from [this tutorial](http://blog.ivank.net/fortunes-algorithm-and-implementation.html) by Ivan Kuckir.

## Installation

To install, clone latest source of Godot Engine, drop the `gdvoronoi` directory in the `godot/modules` directory and compile it.


Command line:
```
git clone https://github.com/godotengine/godot.git
cd godot/modules
git clone https://github.com/jmoon1506/gdvoronoi.git
cd ../../
compile~
```

## Functions:
```
Array generate( int n, int w, int h, int seed )
