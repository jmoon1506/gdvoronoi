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

	/*
		Clip at boundary using Sutherland-Hodgman algorithm

		bound	: pointer to virtual VBoundary
	*/
	void ClipBoundary(VBoundary * bound);

	/*
		Check to remove algorithm errors, vertices >= 3

		w		: graph width
		h 		: graph height
	*/
	bool SanityCheck(double w, double h);

private:
	std::list<VPoint *> 	points;	// list of new points created during boundary clipping

};



#endif