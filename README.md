# GDVoronoi

Implementation of Fortune's Algorithm for Godot Engine

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

## Example

```python
extends Node2D

var vor = Voronoi.new()
var polygon_prefab = preload("res://polygon.tscn")

func _ready():
	var polygons = vor.generate(50, 100, 100, randi())
	for p in polygons:
		var new_poly = polygon_prefab.instance()
		new_poly.set_polygon(Vector2Array(p["corners"]))
		new_poly.set_pos(p["center"])
		new_poly.set_color(Color(randf(), randf(), randf()))
		add_child(new_poly)
```

## Functions:
```
Array generate( int n, int w, int h, int seed )
