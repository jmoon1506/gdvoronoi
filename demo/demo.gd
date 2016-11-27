extends Node2D

var polygon_prefab = preload("res://polygon.tscn")
var vor = Voronoi.new()
var polygons = null
onready var camera = get_node("camera")
var selected

func _ready():
	set_process_input(true)
		
func _input(event):
	if event.type == InputEvent.KEY:
		if event.scancode == KEY_SPACE and event.pressed == false:
			print("new voronoi")
			destroy_voronoi()
			create_voronoi()
		elif event.scancode == KEY_A and event.pressed == false:
			print("VERTICES")
			for v in selected.get_polygon():
				print(v + selected.get_pos())
		elif event.scancode == KEY_B and event.pressed == false:
			print("LOCATION")
			print(get_local_mouse_pos())
			
func create_voronoi():
#	var vseed = 774640868
	var vseed = randi()
	print(vseed)
	polygons = vor.generate(50, 600, 600, vseed)
	for p in polygons:
		var new_poly = polygon_prefab.instance()
		new_poly.initialize(p["corners"], p["center"], Color(randf(), randf(), randf(), 0.5))
		get_node("polygons").add_child(new_poly)
	
func destroy_voronoi():
	for polygon in get_node("polygons").get_children():
		polygon.queue_free()
