#include "recursiveVoronoi.h"
#include <chrono>
#include <random>
#include <iostream>
#define EPSILON std::numeric_limits<double>::epsilon()
#define OFFSET 1.0f
DiscreteGlobalGrid* recursiveVoronoi::begin(std::vector<Vector2> corpos, int level)
{
						bool isaddcor = false;
	std::vector<Vector2> points = generatePoints(Box{ corpos, 0, 0, 0, 0 }, 20, isaddcor);
	
				VoronoiDiagram diagram = getCfCVDiagram(corpos, points);
	
	if (level == 0)
	{
				setDiscreteGlobalGrid(diagram);
		mDiagrams.push_back(std::move(diagram));
	}
	else
	{
		generateVoronoi(diagram, level - 1);
	}
	return nullptr;
}
DiscreteGlobalGrid * recursiveVoronoi::begin_dynamic(std::vector<Vector2> corpos, int level)
{
	bool isaddcor = true;
	std::vector<Vector2> points = generatePoints(Box{ corpos, 0, 0, 0, 0 }, 10, isaddcor);
	VoronoiDiagram diagram = getCfCVDiagram_dynamic(corpos, points);
	if (level == 0)
	{
		setDiscreteGlobalGrid(diagram);
		mDiagrams.push_back(std::move(diagram));
	}
	else
	{
		generateVoronoi(diagram, level - 1);
	}
	return nullptr;
}
#include <stack>
void recursiveVoronoi::generateVoronoi(VoronoiDiagram &lastdiagram, int level)
{
	if (level-- < 0)
	{
		return;
	}
	std::stack<int> q;
	std::vector<bool> visited;
	for (size_t i = 0; i < lastdiagram.getNbSites(); i++)
	{
		visited.push_back(false);
	}
	q.push(0);
	visited[0] = true;
	VoronoiDiagram newdiagram = subVoronoi(lastdiagram, 0);
	while (!q.empty())
	{
		int id = q.top();
		q.pop();
		const VoronoiDiagram::Site* site = lastdiagram.getSite(id);
		Vector2 center = site->point;
		VoronoiDiagram::HalfEdge* halfEdge = site->face->outerComponent;
		VoronoiDiagram::HalfEdge* start = halfEdge;
		while (true)
		{
			if (halfEdge->twin != nullptr)
			{
				int id2 = halfEdge->twin->incidentFace->site->index;
				if (!visited[id2])
				{
					visited[id2] = true;
					q.push(id2);
										VoronoiDiagram diagram = subVoronoi(lastdiagram,id2);
										mergeVoronoi(lastdiagram, newdiagram, diagram);
				}
			}
			halfEdge = halfEdge->next;
			if (halfEdge == start)
				break;
		}
	}
			setDiscreteGlobalGrid(lastdiagram);
	mDiagrams.push_back(std::move(lastdiagram));
	generateVoronoi(newdiagram, level);
}
void recursiveVoronoi::mergeVoronoi(VoronoiDiagram &parentDiagram, VoronoiDiagram &newDiagram, VoronoiDiagram &diagram)
{
	}
DiscreteGlobalGrid recursiveVoronoi::getDiscreteGlobalGrid()
{
	return mDiscreteGlobalGrid;
}
void recursiveVoronoi::setDiscreteGlobalGrid(VoronoiDiagram & diagram)
{
	for (std::size_t i = 0; i < diagram.getNbSites(); ++i)
	{
		const VoronoiDiagram::Site* site = diagram.getSite(i);
		Vector2 center = site->point;
		mDiscreteGlobalGrid.points.push_back(center);
		VoronoiDiagram::HalfEdge* halfEdge = site->face->outerComponent;
		VoronoiDiagram::HalfEdge* start = halfEdge;
		Edge e;
		double testangle = 0;
		while (true && halfEdge != nullptr && halfEdge->origin != nullptr && halfEdge->destination != nullptr)
		{
			testangle += halfEdge->angle;
			Vector2 origin = (halfEdge->origin->point - center) * OFFSET + center;
			Vector2 destination = (halfEdge->destination->point - center) * OFFSET + center;
			e.origin.push_back(origin);
			e.destination.push_back(destination);
			halfEdge = halfEdge->next;
			if (halfEdge == start || halfEdge == nullptr || halfEdge->origin == nullptr || halfEdge->destination == nullptr)
				break;
		}
		mDiscreteGlobalGrid.edges.push_back(e);
	}
	mDiscreteGlobalGrid.otherTestPoints.insert(mDiscreteGlobalGrid.otherTestPoints.end(),
		diagram.mBoundSitesid.begin(), diagram.mBoundSitesid.end());
}
void recursiveVoronoi::clearDiscreteGlobalGrid(VoronoiDiagram & diagram)
{

}
VoronoiDiagram recursiveVoronoi::getCVDiagram(std::vector<Vector2> corpos, std::vector<Vector2> points, bool isadd)
{
	double error = 0.1;
	VoronoiDiagram diagram;
	while (true)
	{
		diagram = getDiagram(corpos, points);
		std::vector<Vector2> newpoints;
		if (isadd)
			newpoints = diagram.getCentroidPos(corpos.size());
		else
			newpoints = diagram.getCentroidPos(0);
				size_t i = 0;
		for (; i < newpoints.size(); i++)
		{
			double x1 = newpoints[i].x;
			double y1 = newpoints[i].y;
			double x2 = points[i].x;
			double y2 = points[i].y;
			double d = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
			if (d > error)
			{
				points = newpoints;
				break;
			}
		}
		if (i == newpoints.size())
			break;
	}
	return std::move(diagram);
}
VoronoiDiagram recursiveVoronoi::getCfCVDiagram(std::vector<Vector2> corpos, std::vector<Vector2> points, bool isadd)
{
	double error = 0.1;
	VoronoiDiagram diagram;
	int n = 0;
	while (true)
	{
		diagram = getDiagram(corpos, points);
		std::vector<Vector2> newpoints = diagram.getCfCVDPos(corpos.size() + n, n);
				size_t i = 0;
		for (; i < newpoints.size(); i++)
		{
			double x1 = newpoints[i].x;
			double y1 = newpoints[i].y;	
			double x2 = points[i].x;
			double y2 = points[i].y;
			double d = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
			if (d > error)
			{
				points = newpoints;
				break;
			}
		}
		if (i == newpoints.size())
			break;
	}
	return std::move(diagram);
}
VoronoiDiagram recursiveVoronoi::getCfCVDiagram_dynamic(std::vector<Vector2> corpos, std::vector<Vector2> points, bool isadd)
{
	double error = 0.1;
	VoronoiDiagram diagram;
	int n = 0;
	while (true)
	{
		diagram = getDiagram(corpos, points);
		setDiscreteGlobalGrid(diagram);
		std::vector<Vector2> newpoints = diagram.getCfCVDPos(corpos.size() + n, n);
		size_t i = 0;
		for (; i < newpoints.size(); i++)
		{
			double x1 = newpoints[i].x;
			double y1 = newpoints[i].y;
			double x2 = points[i].x;
			double y2 = points[i].y;
			double d = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
			if (d > error)
			{
				points = newpoints;
				break;
			}
		}
		if (i == newpoints.size())
			break;
	}
	return std::move(diagram);
}
VoronoiDiagram recursiveVoronoi::getDiagram(std::vector<Vector2> corpos, std::vector<Vector2> points)
{
	FortuneAlgorithm algorithm(points);
	algorithm.construct();
	algorithm.bound(Box{ corpos, 0.0, 0.0, 0.0, 0.0 });
	VoronoiDiagram diagram = algorithm.getDiagram();
	bool valid = diagram.intersect(Box{ corpos, 0.0, 0.0, 0.0, 0.0 });
	if (!valid)
		throw std::runtime_error("An error occured in the box intersection algorithm");
	return std::move(diagram);
}
VoronoiDiagram recursiveVoronoi::subVoronoi(VoronoiDiagram &lastdiagram,int id)
{
	std::vector<Vector2> corpos;
	const VoronoiDiagram::Site* site = lastdiagram.getSite(id);
	VoronoiDiagram::HalfEdge* halfEdge = site->face->outerComponent;
	VoronoiDiagram::HalfEdge* start = halfEdge;
	while (true)
	{
		corpos.push_back(halfEdge->origin->point);
		halfEdge = halfEdge->next;
		if (halfEdge == start)
			break;
	}
	std::vector<Vector2> points = generatePoints(Box{ corpos, 0, 0, 0, 0 }, 10,true);
			VoronoiDiagram diagram = getCfCVDiagram(corpos, points);
	setDiscreteGlobalGrid(diagram);
	return std::move(diagram);
}

std::vector<Vector2> recursiveVoronoi::generatePoints(Box box, int nbPoints, bool isAddCor)
{
	uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	box.left = box.corpos[0].x;
	box.bottom = box.corpos[0].y;
	box.right = box.corpos[0].x;
	box.top = box.corpos[0].y;
	for (size_t i = 0; i < box.corpos.size(); i++)
	{
		box.left = std::min(box.corpos[i].x, box.left);
		box.bottom = std::min(box.corpos[i].y, box.bottom);
		box.right = std::max(box.corpos[i].x, box.right);
		box.top = std::max(box.corpos[i].y, box.top);
	}
	double w = box.right - box.left;
	double h = box.top - box.bottom;

	std::vector<Vector2> points;
	while (points.size() < nbPoints)
	{
		Vector2 pos(box.left + w * (double)rand() / (double)RAND_MAX, box.bottom + h * (double)rand() / (double)RAND_MAX);
		if (box.contains(pos) >= 0)
		{
			bool ret = true;
			for (size_t i = 0; i < points.size(); i++)
			{
				if (std::abs(points[i].y - pos.y) < EPSILON)
				{
					ret = false;
					break;
				}
			}
			if (ret)
			{
				points.push_back(pos);
			}
		}
	}
	if (isAddCor)
	{
		for (size_t i = 0; i < box.corpos.size(); i++)
		{
			points.push_back(box.corpos[i]);
		}
	}
	return points;
}