/* gdvoronoi.cpp */

#include "gdvoronoi.h"
#include "globals.h"
#include "os/os.h"
#include "voronoi/VGraph.h"
#include "voronoi/VPoint.h"
#include "math_2d.h"
#include "ustring.h"

Voronoi::Voronoi() {
}

Array Voronoi::generate(int n, int w, int h, int seed) {
	Array result;
	vor::VGraph * v = new vor::VGraph();
	vor::Vertices * ver = new vor::Vertices();

	srand ( seed );
	for(int i=0; i<n; i++)
	{
		ver->push_back(new VPoint( w * (double)rand()/(double)RAND_MAX , h * (double)rand()/(double)RAND_MAX )); 
	}
	v->GetEdges(ver, w, h);
	vor::Polygons * pol = v->GetPolygons();
	for(vor::Polygons::iterator i = pol->begin(); i!= pol->end(); ++i)
	{
		Dictionary entry;
		entry[String::utf8("center")] = Vector2((*i)->center->x, (*i)->center->y);
		Vector2Array corners;
		for (std::list<VPoint *>::iterator j = (*i)->vertices.begin(); j!= (*i)->vertices.end(); ++j) {
			corners.push_back( Vector2(
				std::min(std::max((*j)->x, 0), w) - w/2,
				std::min(std::max((*j)->y, 0), h) - h/2 );
		}
		entry[String::utf8("corners")] = corners;
		result.push_back(entry);
	}
	delete v;
	while(!ver->empty()) delete ver->front(), ver->pop_front();
	delete ver;
	return result;
}

void Voronoi::_bind_methods() {
	ObjectTypeDB::bind_method("generate", &Voronoi::generate);
}

