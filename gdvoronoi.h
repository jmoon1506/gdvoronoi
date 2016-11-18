/* gdvoronoi.h */
#ifndef GDVORONOI_H
#define GDVORONOI_H

#include "reference.h"
#include "voronoi/VGraph.h"

using namespace std;

// Godot bindings
class Voronoi : public Reference {
	OBJ_TYPE(Voronoi,Reference);

protected:
	static void _bind_methods();

public:
	Voronoi();
    Array generate(int n, int w, int h, int seed);
};

#endif
