/* gdvoronoi.h */
#ifndef GDVORONOI_H
#define GDVORONOI_H

#include "reference.h"

// Godot bindings
class Voronoi : public Reference {
	OBJ_TYPE(Voronoi,Reference);

protected:
	static void _bind_methods();

public:
	Voronoi();
    Array generate(int n, double w, double h, int seed);
    Array generate_from_points(Vector2Array points, double w, double h);
};

#endif
