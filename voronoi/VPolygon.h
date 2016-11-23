#ifndef VPolygon_h
#define VPolygon_h

#include "VEdge.h"
#include "VBoundary.h"
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
	~VPolygon();
	void AddEdge(VEdge * e) { edges.push_back(e); }

	void SetVertices();
	void ClipBoundary(VBoundary * bound);

private:
	std::list<VPoint *> 	points;	// list of new points created during boundary clipping

};



#endif