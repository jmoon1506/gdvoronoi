extends Polygon2D

var default_color = Color(1, 1, 1)
var clicked = false
var demo

func _ready():
	demo = get_node("../..")

func initialize(corners, center, color):
	self.set_pos(center)
	var verts = Vector2Array()
	for v in corners:
		verts.append(v - center)
	set_polygon(verts)
#	var shape = ConvexPolygonShape2D.new()
#	shape.set_points(verts)
#	get_node("area/shape").set_shape(shape)
	set_color(color)
	default_color = color
	
func _on_area_input_event( viewport, event, shape_idx ):
	if event.type == InputEvent.MOUSE_BUTTON && event.button_index == BUTTON_LEFT && event.pressed:
		print("collide!")
		if not clicked:
			set_color(default_color.linear_interpolate(Color(1,1,1),0.2))
			clicked = true
			demo.selected = self
		else:
			set_color(default_color)
			clicked = false
