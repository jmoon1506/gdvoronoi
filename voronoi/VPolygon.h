#ifndef VPolygon_h
#define VPolygon_h

#include "VEdge.h"
#include <list>
#include <algorithm>

class VPoint;
class VEdge;
/*
    A class that stores a Voronoi region as a polygon

    center      : pointer to center point
    corners     : list of corner points
    neighbors	: list of adjacent regions
*/
class VPolygon
{
public:

	VPoint *				center;
	std::list<VEdge *>		edges;
	// std::list<VPolygon *> 	neighbors;
	std::list<VPoint *>		vertices;

	VPolygon (VPoint * c) { center = c; }
	void AddEdge(VEdge * e) { edges.push_back(e); }

	void SetVertices() {
		for(std::list<VEdge *>::iterator i = edges.begin(); i!= edges.end(); ++i)
		{
			bool start_exists = std::any_of( std::begin( vertices ), std::end( vertices ),
							[&](VPoint * arg){return arg == (*i)->start;} );
			bool end_exists = std::any_of( std::begin( vertices ), std::end( vertices ),
							[&](VPoint * arg){return arg == (*i)->end;} );
			if (!start_exists)
				vertices.push_back((*i)->start);
			if (!end_exists)
				vertices.push_back((*i)->end);
			// std::cout << (*i)->edges.size() << "\n";
		}
		vertices.sort([&](VPoint * a, VPoint * b){
			if (a->x - center->x >= 0 && b->x - center->x < 0)
				return true;
			if (a->x - center->x < 0 && b->x - center->x >= 0)
				return false;
			if (a->x - center->x == 0 && b->x - center->x == 0) {
				if (a->y - center->y >= 0 || b->y - center->y >= 0)
					return a->y > b->y;
				return b->y > a->y;
			}

			// compute the cross product of vectors (center -> a) x (center -> b)
			int det = (a->x - center->x) * (b->y - center->y) - (b->x - center->x) * (a->y - center->y);
			if (det < 0)
				return true;
			if (det > 0)
				return false;

			// points a and b are on the same line from the center
			// check which point is closer to the center
			int d1 = (a->x - center->x) * (a->x - center->x) + (a->y - center->y) * (a->y - center->y);
			int d2 = (b->x - center->x) * (b->x - center->x) + (b->y - center->y) * (b->y - center->y);
			return d1 > d2;		
		} );
	};
};




#endif