
#include "VGraph.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <cmath>
#include <map>
#include <vector>

using namespace vor;

VGraph::VGraph()
{
	edges = 0;
	coherent = true;
}

VGraph::~VGraph()
{
	for(Vertices::iterator	i = points.begin(); i != points.end(); ++i) delete (*i);
	for(Edges::iterator		i = edges->begin(); i != edges->end(); ++i) delete (*i);
	for(Polygons::iterator	i = polygons->begin(); i != polygons->end(); ++i) delete (*i);
	for(Vertices::iterator  i = lloyd_vertices->begin(); i != lloyd_vertices->end(); ++i) delete (*i);
	delete lloyd_vertices;
	delete edges;
	delete polygons;
}

void VGraph::GetEdges(Vertices * v, double w, double h)
{
	places = v;
	width = w;
	height = h;
	root = 0;
	coherent = true;

	if(!edges) edges = new Edges();
	else 
	{
		for(Vertices::iterator	i = points.begin(); i != points.end(); ++i) delete (*i);
		for(Edges::iterator		i = edges->begin(); i != edges->end(); ++i) delete (*i);
		points.clear();
		edges->clear();
	}
	
	for(Vertices::iterator i = places->begin(); i!=places->end(); ++i)
	{
		queue.push(new VEvent( *i, true));
	}

	VEvent * e;
	while(!queue.empty())
	{
		e = queue.top();
		queue.pop();
		ly = e->point->y;
		if(deleted.find(e) != deleted.end()) { delete(e); deleted.erase(e); continue;}
		if(e->pe) InsertParabola(e->point);
		else RemoveParabola(e);
		delete(e);
	}
	
	FinishEdge(root);

	for(Edges::iterator i = edges->begin(); i != edges->end(); ++i)
	{
		if( (*i)->neighbour) 
		{
			(*i)->start = (*i)->neighbour->end;
			delete (*i)->neighbour;
		}
	}

	/*
		Create map of Voronoi points to boundary vertices.
	*/

	std::vector<VBoundary*> boundaries;
	boundaries.push_back(new VBoundaryHorz(0, true));
	boundaries.push_back(new VBoundaryHorz(height, false));
	boundaries.push_back(new VBoundaryVert(0, true));
	boundaries.push_back(new VBoundaryVert(width, false));

	std::map<VPoint *, std::vector<VPoint *>> bound_map;

	for(Edges::iterator i = edges->begin(); i != edges->end(); ++i)
	{
		for(std::vector<VBoundary*>::iterator j = boundaries.begin(); j != boundaries.end(); ++j)
		{
			if ((*j)->IsInside((*i)->start))
			{
				if (!(*j)->IsInside((*i)->end)) {

					(*i)->end = (*j)->Intersect((*i)->start, (*i)->end);

					if ((*i)->end->x >= 0 && (*i)->end->x <= w && (*i)->end->y >= 0 && (*i)->end->y <= h) {
						if (bound_map.find((*i)->left) == bound_map.end()) {
							bound_map[(*i)->left] = { (*i)->end };
						} else {
							bound_map[(*i)->left].push_back((*i)->end);
						}

						if (bound_map.find((*i)->right) == bound_map.end()) {
							bound_map[(*i)->right] = { (*i)->end };
						} else {
							bound_map[(*i)->right].push_back((*i)->end);
						}
					}
				}
			}
			else if ((*j)->IsInside((*i)->end)) {
				(*i)->start = (*j)->Intersect((*i)->start, (*i)->end);

				if ((*i)->start->x >= 0 && (*i)->start->x <= w && (*i)->start->y >= 0 && (*i)->start->y <= h) {
					if (bound_map.find((*i)->left) == bound_map.end()) {
						bound_map[(*i)->left] = { (*i)->start };
					} else {
						bound_map[(*i)->left].push_back((*i)->start);
					}

					if (bound_map.find((*i)->right) == bound_map.end()) {
						bound_map[(*i)->right] = { (*i)->start };
					} else {
						bound_map[(*i)->right].push_back((*i)->start);
					}
				}
			}
			else {
				delete (*i);
				edges->erase(i--);
				break;
			}
		}
	}
	for(std::vector<VBoundary*>::iterator i = boundaries.begin(); i != boundaries.end(); ++i) delete (*i);

	/*
		Convert boundary vertices to edges.
	*/

	for(std::map<VPoint *, std::vector<VPoint *>>::const_iterator i=bound_map.begin(); i!=bound_map.end(); ++i) {
		if (i->second.size() == 2) {
			if (!((i->second[0]->x == i->second[1]->x) || (i->second[0]->y == i->second[1]->y))) {
				double px, py;
				if (i->second[0]->x == 0 || i->second[1]->x == 0) {
					px = 0;
				} else {
					px = w;
				}
				if (i->second[0]->y == 0 || i->second[1]->y == 0) {
					py = 0;
				} else {
					py = h;
				}
				VPoint * corner = new VPoint(px, py);
				points.push_back(corner);
				VEdge * boundary_edge1 = new VEdge(i->second[0], corner, i->first, NULL);
				edges->push_back(boundary_edge1);
				VEdge * boundary_edge2 = new VEdge(i->second[1], corner, i->first, NULL);
				edges->push_back(boundary_edge2);
				// std::cout << "Corner!\n";
			}
			else {
				VEdge * boundary_edge = new VEdge(i->second[0], i->second[1], i->first, NULL);
				edges->push_back(boundary_edge);
				// std::cout << "Edge!\n";
			}
		} else if (i->second.size() == 4) {
			int h1 = -1;
			int h2 = -1;
			int v1 = -1;
			int v2 = -1;
			for(int k = 0; k < 4; ++k){
				if (i->second[k]->x == 0 || i->second[k]->x == w) {
					if (h1 == -1) h1 = k;
					else h2 = k;
				} else if (i->second[k]->y == 0 || i->second[k]->y == h) {
					if (v1 == -1) v1 = k;
					else v2 = k;
				} else {
					std::cout << "Error: Invalid boundary straddling vertex.\n";
					coherent = false;
				}
			}

			if (h1 == -1 || h2 == -1 || v1 == -1 || v2 == -1) {
				std::cout << "Error: Invalid boundary straddling vertices.\n";
/*				std::cout << "h1: " << h1 << ", h2: " << h2 << ", v1: " << v1 << ", v2: " << v2 << "\n";
				for(int k = 0; k < 4; ++k){
					std::cout << i->second[k]->x << ", " << i->second[k]->y << "\n";
				}*/
				coherent = false;
			} else {
				VEdge * boundary_edge1 = new VEdge(i->second[h1], i->second[h2], i->first, NULL);
				edges->push_back(boundary_edge1);
				VEdge * boundary_edge2 = new VEdge(i->second[v1], i->second[v2], i->first, NULL);
				edges->push_back(boundary_edge2);
			}
		} else {
			std::cout << "Error: Boundary edge has " << i->second.size() << " vertices (should have 2).\n";
/*			std::cout << "Location: " << -1.0 + 2*i->first->x/w << ", " << -1.0 + 2*i->first->y/h << "\n";
			std::cout << "Vertices: \n";
			for(auto k = i->second.begin(); k != i->second.end(); ++k) {
				std::cout << -1.0 + 2*(*k)->x/w << ", " << -1.0 + 2*(*k)->y/h << "\n";
			}*/
			coherent = false;
		}
		std::cout << "Location: " << -1.0 + 2*i->first->x/w << ", " << -1.0 + 2*i->first->y/h << "\n";
		std::cout << "Vertices: \n";
		for(auto k = i->second.begin(); k != i->second.end(); ++k) {
			std::cout << -1.0 + 2*(*k)->x/w << ", " << -1.0 + 2*(*k)->y/h << "\n";
		}
	}
}

Polygons * VGraph::CalculatePolygons(Vertices * v, double w, double h)
{
	GetEdges(v, w, h);

	std::map<VPoint *, VPolygon *> pol_map;
	for(Edges::iterator i = edges->begin(); i != edges->end(); ++i)
	{
		if ((*i)->left != NULL) {
			if(!pol_map[(*i)->left]){ pol_map[(*i)->left] = new VPolygon((*i)->left); }
			pol_map[(*i)->left]->AddEdge(*i);
		}
		if ((*i)->right != NULL) {
			if(!pol_map[(*i)->right]){ pol_map[(*i)->right] = new VPolygon((*i)->right); }
			pol_map[(*i)->right]->AddEdge(*i);
		}
	}

	if(!polygons) polygons = new Polygons();
	else 
	{
		for(Polygons::iterator i = polygons->begin(); i != polygons->end(); ++i) delete (*i);
		polygons->clear();
	}
	
	for(std::map<VPoint *, VPolygon *>::const_iterator i=pol_map.begin(); i!=pol_map.end(); ++i) {
		i->second->SetVertices();
		polygons->push_back( i->second );
	}
	return polygons;
}

Vertices * VGraph::LloydRelaxation() {
	if (!lloyd_vertices) {
		lloyd_vertices = new Vertices;
	} else {
		for(Vertices::iterator i = lloyd_vertices->begin(); i != lloyd_vertices->end(); ++i) delete (*i);
		lloyd_vertices->clear();
	}
	for(Polygons::iterator i = polygons->begin(); i != polygons->end(); ++i) {
		double x = 0;
		double y = 0;
		for(std::list<VPoint *>::iterator j = (*i)->vertices.begin(); j != (*i)->vertices.end(); ++j) {
			x += (*j)->x;
			y += (*j)->y;
		}
		x = x / (*i)->vertices.size();
		y = y / (*i)->vertices.size();
		lloyd_vertices->push_back(new VPoint(x, y));
	}
	CalculatePolygons(lloyd_vertices, width, height);
	return lloyd_vertices;
}

void VGraph::EaseCorners() {
	
}

void	VGraph::InsertParabola(VPoint * p)
{
	if(!root){root = new VParabola(p); return;}

	if(root->isLeaf && root->site->y - p->y < 1) // degenerovaný pøípad - obì spodní místa ve stejné výšce
	{
		VPoint * fp = root->site;
		root->isLeaf = false;
		root->SetLeft( new VParabola(fp) );
		root->SetRight(new VParabola(p)  );
		VPoint * s = new VPoint((p->x + fp->x)/2, height); // zaèátek hrany uprostøed míst
		points.push_back(s);
		if(p->x > fp->x) root->edge = new VEdge(s, fp, p); // rozhodnu, který vlevo, který vpravo
		else root->edge = new VEdge(s, p, fp);
		edges->push_back(root->edge);
		return;
	}

	VParabola * par = GetParabolaByX(p->x);
	
	if(par->cEvent)
	{
		deleted.insert(par->cEvent);
		par->cEvent = 0;
	}
	
	VPoint * start = new VPoint(p->x, GetY(par->site, p->x));
	points.push_back(start);

	VEdge * el = new VEdge(start, par->site, p);
	VEdge * er = new VEdge(start, p, par->site);

	el->neighbour = er;
	edges->push_back(el);

	// pøestavuju strom .. vkládám novou parabolu
	par->edge = er;
	par->isLeaf = false;

	VParabola * p0 = new VParabola(par->site);
	VParabola * p1 = new VParabola(p);
	VParabola * p2 = new VParabola(par->site);

	par->SetRight(p2);
	par->SetLeft(new VParabola());
	par->Left()->edge = el;

	par->Left()->SetLeft(p0);
	par->Left()->SetRight(p1);
	
	CheckCircle(p0);
	CheckCircle(p2);
}

void	VGraph::RemoveParabola(VEvent * e)
{
	VParabola * p1 = e->arch;

	VParabola * xl = VParabola::GetLeftParent(p1);
	VParabola * xr = VParabola::GetRightParent(p1);

	VParabola * p0 = VParabola::GetLeftChild(xl);
	VParabola * p2 = VParabola::GetRightChild(xr);

	if(p0 == p2) std::cout << "chyba - pravá a levá parabola má stejné ohnisko!\n";

	if(p0->cEvent){ deleted.insert(p0->cEvent); p0->cEvent = 0; }
	if(p2->cEvent){ deleted.insert(p2->cEvent); p2->cEvent = 0; }

	VPoint * p = new VPoint(e->point->x, GetY(p1->site, e->point->x));
	points.push_back(p);

	xl->edge->end = p;
	xr->edge->end = p;
	
	VParabola * higher;
	VParabola * par = p1;
	while(par != root)
	{
		par = par->parent;
		if(par == xl) higher = xl;
		if(par == xr) higher = xr;
	}
	higher->edge = new VEdge(p, p0->site, p2->site);
	edges->push_back(higher->edge);

	VParabola * gparent = p1->parent->parent;
	if(p1->parent->Left() == p1)
	{
		if(gparent->Left()  == p1->parent) gparent->SetLeft ( p1->parent->Right() );
		if(gparent->Right() == p1->parent) gparent->SetRight( p1->parent->Right() );
	}
	else
	{
		if(gparent->Left()  == p1->parent) gparent->SetLeft ( p1->parent->Left()  );
		if(gparent->Right() == p1->parent) gparent->SetRight( p1->parent->Left()  );
	}

	delete p1->parent;
	delete p1;

	CheckCircle(p0);
	CheckCircle(p2);
}

void	VGraph::FinishEdge(VParabola * n)
{
	if(n->isLeaf) {delete n; return;}
	double mx;
	if(n->edge->direction->x > 0.0)	mx = std::max(width,	n->edge->start->x + 10 );
	else							mx = std::min(0.0,		n->edge->start->x - 10);
	
	VPoint * end = new VPoint(mx, mx * n->edge->f + n->edge->g); 
	n->edge->end = end;
	points.push_back(end);
			
	FinishEdge(n->Left() );
	FinishEdge(n->Right());
	delete n;
}

double	VGraph::GetXOfEdge(VParabola * par, double y)
{
	VParabola * left = VParabola::GetLeftChild(par);
	VParabola * right= VParabola::GetRightChild(par);

	VPoint * p = left->site;
	VPoint * r = right->site;

	double dp = 2.0 * (p->y - y);
	double a1 = 1.0 / dp;
	double b1 = -2.0 * p->x / dp;
	double c1 = y + dp / 4 + p->x * p->x / dp;
			
		   dp = 2.0 * (r->y - y);
	double a2 = 1.0 / dp;
	double b2 = -2.0 * r->x/dp;
	double c2 = ly + dp / 4 + r->x * r->x / dp;
			
	double a = a1 - a2;
	double b = b1 - b2;
	double c = c1 - c2;
			
	double disc = b*b - 4 * a * c;
	double x1 = (-b + std::sqrt(disc)) / (2*a);
	double x2 = (-b - std::sqrt(disc)) / (2*a);

	double ry;
	if(p->y < r->y ) ry =  std::max(x1, x2);
	else ry = std::min(x1, x2);

	return ry;
}

VParabola * VGraph::GetParabolaByX(double xx)
{
	VParabola * par = root;
	double x = 0.0;

	while(!par->isLeaf) // projdu stromem dokud nenarazím na vhodný list
	{
		x = GetXOfEdge(par, ly);
		if(x>xx) par = par->Left();
		else par = par->Right();				
	}
	return par;
}

double	VGraph::GetY(VPoint * p, double x) // ohnisko, x-souøadnice
{
	double dp = 2 * (p->y - ly);
	double a1 = 1 / dp;
	double b1 = -2 * p->x / dp;
	double c1 = ly + dp / 4 + p->x * p->x / dp;
	
	return(a1*x*x + b1*x + c1);
}

void	VGraph::CheckCircle(VParabola * b)
{
	VParabola * lp = VParabola::GetLeftParent (b);
	VParabola * rp = VParabola::GetRightParent(b);

	VParabola * a  = VParabola::GetLeftChild (lp);
	VParabola * c  = VParabola::GetRightChild(rp);

	if(!a || !c || a->site == c->site) return;

	VPoint * s = 0;
	s = GetEdgeIntersection(lp->edge, rp->edge);
	if(s == 0) return;

	double dx = a->site->x - s->x;
	double dy = a->site->y - s->y;

	double d = std::sqrt( (dx * dx) + (dy * dy) );

	if(s->y - d >= ly) { return;}

	VEvent * e = new VEvent(new VPoint(s->x, s->y - d), false);
	points.push_back(e->point);
	b->cEvent = e;
	e->arch = b;
	queue.push(e);
}

VPoint * VGraph::GetEdgeIntersection(VEdge * a, VEdge * b)
{
	double x = (b->g-a->g) / (a->f - b->f);
	double y = a->f * x + a->g;

	if((x - a->start->x)/a->direction->x < 0) return 0;
	if((y - a->start->y)/a->direction->y < 0) return 0;
		
	if((x - b->start->x)/b->direction->x < 0) return 0;
	if((y - b->start->y)/b->direction->y < 0) return 0;	

	VPoint * p = new VPoint(x, y);		
	points.push_back(p);
	return p;
}


	