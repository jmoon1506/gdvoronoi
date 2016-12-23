#ifndef VGraph_h
#define VGraph_h

#include <list>
#include <queue>
#include <set>

#include "VPoint.h"
#include "VEdge.h"
#include "VParabola.h"
#include "VEvent.h"
#include "VPolygon.h"
#include "VBoundary.h"
#include <functional>


namespace vor
{
	/*
		Useful data containers for Vertices (places) and Edges of Voronoi diagram
	*/

	typedef std::list<VPoint *>		Vertices	;
	typedef std::list<VEdge *>		Edges		;
	typedef std::list<VPolygon *>	Polygons	;

	/*
		Class for generating the Voronoi diagram
	*/

	class VGraph
	{
	public:
		
		/*
			Constructor - without any parameters
		*/

		VGraph();
		~VGraph();

		/*
						CalculatePolygons 		: the only public function for generating a diagram
						
											input:
												v		: Vertices - places for drawing a diagram
												w		: width  of the result (top left corner is (0, 0))
												h		: height of the result
											output:
												pointer to list of edges
											
											All the data structures are managed by this class

						IsCoherent 			: checks for irregularities during edge creation
						LloydRelaxation		: recalculates graph using polygon centroids
						EaseCorners			: moves corners to centroid of neighboring polygon centers (technically no longer a Voronoi graph)
		*/

		/*
						ConstrainPoint		: constrain vertices within boundaries
						InsertParabola		: processing the place event
						RemoveParabola		: processing the circle event
						FinishEdge			: recursively finishes all infinite edges in the tree
						GetXOfEdge			: returns the current x position of an intersection point of left and right parabolas
						GetParabolaByX		: returns the Parabola that is under this "x" position in the current beachline
						CheckCircle			: checks the circle event (disappearing) of this parabola
						GetEdgeInterse
		*/

		Polygons *		CalculatePolygons(Vertices * v, double w, double h);
		bool 			IsCoherent() { return coherent; }
		Vertices * 		LloydRelaxation();
		void			EaseCorners();
		Polygons *		polygons;

	private:

		/*
						places				: container of places with which we work
						edges				: container of edges which will be teh result
						width				: width of the diagram
						height				: height of the diagram
						root				: the root of the tree, that represents a beachline sequence
						ly					: current "y" position of the line (see Fortune's algorithm)
						coherent 			: indicates whether edges messed up in GetEdges()
						lloyd_vertices		: list of vertices created from Lloyd relaxation
		*/

		Vertices *		places;
		Edges *			edges;
		double			width, height;
		VParabola *		root;
		double			ly;
		bool 			coherent;
		Vertices * 		lloyd_vertices;


		/*
						deleted				: set  of deleted (false) Events (since we can not delete from PriorityQueue
						points				: list of all new points that were created during the algorithm
						queue				: priority queue with events to process
		*/

		std::set<VEvent *>	deleted;
		std::list<VPoint *> points;
		std::priority_queue<VEvent *, std::vector<VEvent *>, VEvent::CompareEvent> queue;

		/*
						GetEdges 			: master function for Fortune's algorithm; also applies Sutherland edge clipping
						ConstrainPoint		: constrain vertices within boundaries
						InsertParabola		: processing the place event
						RemoveParabola		: processing the circle event
						FinishEdge			: recursively finishes all infinite edges in the tree
						GetXOfEdge			: returns the current x position of an intersection point of left and right parabolas
						GetParabolaByX		: returns the Parabola that is under this "x" position in the current beachline
						CheckCircle			: checks the circle event (disappearing) of this parabola
						GetEdgeIntersection	: helper function for CheckCircle
		*/
		void		GetEdges(Vertices * v, double w, double h);
		void		InsertParabola(VPoint * p);
		void		RemoveParabola(VEvent * e);
		void		FinishEdge(VParabola * n);
		double		GetXOfEdge(VParabola * par, double y);
		VParabola * GetParabolaByX(double xx);
		double		GetY(VPoint * p, double x);
		void		CheckCircle(VParabola * b);
		VPoint *	GetEdgeIntersection(VEdge * a, VEdge * b);
	};
}

#endif