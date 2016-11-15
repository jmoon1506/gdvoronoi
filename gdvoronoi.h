/* gdvoronoi.h */
#ifndef GDVORONOI_H
#define GDVORONOI_H

#include "reference.h"
#include "ustring.h"
#include "voronoi/Voronoi.h"

using namespace std;

// Godot bindings
class Voronoi : public Reference {
	OBJ_TYPE(Voronoi,Reference);

protected:
	static void _bind_methods();

public:
	Voronoi();
    void generate_random_points();
    void generate_voronoi();
};

#endif
