#ifndef VBoundary_h
#define VBoundary_h

#include "VPoint.h"

class VBoundary {
public:
	VBoundary(double a, bool gt): pos(a), greaterThan(gt) {}
	virtual bool IsInside(VPoint * p) = 0;
	virtual VPoint * Intersect(VPoint * p0, VPoint * p1) = 0;
	double pos;			
	bool greaterThan;	// true=values greater than pos are inside, false=values lesser than pos are outside
};
class VBoundaryHorz : public VBoundary {
public:
	VBoundaryHorz(double a, bool gt) : VBoundary(a, gt) {}
	bool IsInside(VPoint * p) { return greaterThan ? p->y > pos : p->y < pos; }
	VPoint * Intersect(VPoint * p0, VPoint * p1) {
		double xslope = (p0->x - p1->x) / (p0->y - p1->y);
		VPoint * r = new VPoint(p0->x + xslope * (pos - p0->y), pos);
		return r;
	}
};
class VBoundaryVert : public VBoundary {
public:
	VBoundaryVert(double a, bool gt): VBoundary(a, gt) {}
	bool IsInside(VPoint * p) { return greaterThan ? p->x > pos : p->x < pos; }
	VPoint * Intersect(VPoint * p0, VPoint * p1) {
		double yslope = (p0->y - p1->y) / (p0->x - p1->x);
		VPoint * r = new VPoint(pos, p0->y + yslope * (pos - p0->x));
		return r;
	}
};

#endif