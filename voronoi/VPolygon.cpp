#include "VPolygon.h"

VPolygon::~VPolygon() {
	for(std::list<VPoint *>::iterator	i = points.begin(); i != points.end(); ++i) delete (*i);
}

void VPolygon::SetVertices() {
	// add vertices from each edge, checking if it already exists
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
}

/*void VPolygon::ClipBoundaries(double w, double h) {
	std::list<VPoint *> new_vertices;
	std::list<VPoint *>::iterator prev=vertices.end();
	prev--;
	auto WithinTop = [&w, &h](VPoint * p) { return p->y < h; };
	auto WithinBottom = [&w, &h](VPoint * p) { return p->y > 0; };
	auto WithinLeft = [&w, &h](VPoint * p) { return p->x > 0; };
	auto WithinRight = [&w, &h](VPoint * p) { return p->x < w; };
	auto IntersectTop = [&](VPoint * p0, VPoint * p1) {
		double xslope = (p0->x - p1->x) / (p0->y - p1->y);
		VPoint * r = new VPoint(h, p0->x + xslope * (h - p0->y));
		return r;
	};
	auto IntersectBottom = [&](VPoint * p0, VPoint * p1) {
		double xslope = (p0->x - p1->x) / (p0->y - p1->y);
		VPoint * r = new VPoint(0, p0->x + xslope * (0 - p0->y));
		return r;
	};
	auto IntersectLeft = [&](VPoint * p0, VPoint * p1) {
		double yslope = (p0->y - p1->y) / (p0->x - p1->x);
		VPoint * r = new VPoint(0, p0->y + yslope * (0 - p0->x));
		return r;
	};
	auto IntersectRight = [&](VPoint * p0, VPoint * p1) {
		double yslope = (p0->y - p1->y) / (p0->x - p1->x);
		VPoint * r = new VPoint(w, p0->y + yslope * (w - p0->x));
		return r;
	};
	for(std::list<VPoint *>::iterator cur=vertices.begin(); cur!=vertices.end(); prev=cur, ++cur)
	{
		if (WithinTop(*cur))
		{
			if (WithinTop(*prev))
			{
				new_vertices.push_back(*cur);
			}
			else
			{
				VPoint * r = IntersectTop(*prev, *cur);
				points.push_back(r);
				new_vertices.push_back(r);
				new_vertices.push_back(*cur);
			}
		}
		else if (WithinTop(*prev))
		{
			VPoint * r = IntersectTop(*prev, *cur);
			points.push_back(r);
			new_vertices.push_back(r);
		}
	}
	vertices = new_vertices;
}*/

void VPolygon::ClipBoundary(VBoundary * bound) {
	std::list<VPoint *> new_vertices;
	std::list<VPoint *>::iterator prev=vertices.end();
	prev--;
	for(std::list<VPoint *>::iterator cur=vertices.begin(); cur!=vertices.end(); prev=cur, ++cur)
	{
		if (bound->IsInside(*cur) == true)
		{
			if (bound->IsInside(*prev))
			{
				new_vertices.push_back(*cur);
			}
			else
			{
				VPoint * r = bound->Intersect(*prev, *cur);
				points.push_back(r);
				new_vertices.push_back(r);
				new_vertices.push_back(*cur);
			}
		}
		else if (bound->IsInside(*prev))
		{
			VPoint * r = bound->Intersect(*prev, *cur);
			points.push_back(r);
			new_vertices.push_back(r);
		}
	}
	vertices = new_vertices;
}

