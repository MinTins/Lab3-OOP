

#include "VoronoiDiagram.h"
#include <unordered_set>
#include <iostream>
#include <random>
#define EPSILON 0.00000001


VoronoiDiagram::VoronoiDiagram(const std::vector<Vector2>& points)
{
    mSites.reserve(points.size());
    mFaces.reserve(points.size());
    for(std::size_t i = 0; i < points.size(); ++i)
    {
        mSites.push_back(VoronoiDiagram::Site{i, points[i], nullptr});
        mFaces.push_back(VoronoiDiagram::Face{&mSites.back(), nullptr});
        mSites.back().face = &mFaces.back();
    }
}

VoronoiDiagram::VoronoiDiagram()
{
}

VoronoiDiagram::Site* VoronoiDiagram::getSite(std::size_t i)
{
    return &mSites[i];
}

std::size_t VoronoiDiagram::getNbSites() const
{
    return mSites.size();
}

VoronoiDiagram::Face* VoronoiDiagram::getFace(std::size_t i)
{
    return &mFaces[i];
}

const std::list<VoronoiDiagram::Vertex>& VoronoiDiagram::getVertices() const
{
    return mVertices;
}

const std::list<VoronoiDiagram::HalfEdge>& VoronoiDiagram::getHalfEdges() const
{
    return mHalfEdges;
}

bool VoronoiDiagram::intersect(Box box)
{
    bool error = false;
    std::unordered_set<HalfEdge*> processedHalfEdges;
    std::unordered_set<Vertex*> verticesToRemove;
	int vaildsize = mSites.size() - box.corpos.size();
    for (const Site& site : mSites)
    {
        HalfEdge* halfEdge = site.face->outerComponent;
		int ret = box.contains(halfEdge->origin->point);
		bool inside;
		if (ret == -1)
			inside = false;
		else
			inside = true;
        bool outerComponentDirty = !inside;
        HalfEdge* incomingHalfEdge = nullptr;         HalfEdge* outgoingHalfEdge = nullptr;         int incomingSide, outgoingSide;
        do
        {
            std::array<Box::Intersection, 2> intersections;
            int nbIntersections = box.getIntersections(halfEdge->origin->point, halfEdge->destination->point, intersections);
						int ret2 = box.contains(halfEdge->destination->point);
			bool nextInside;
			if (ret2 == -1)
				nextInside = false;
			else
				nextInside = true;
			
            HalfEdge* nextHalfEdge = halfEdge->next;
			bool isremove = false;
                        if (!inside && !nextInside)
            {
                                if (nbIntersections == 0)
                {
                    verticesToRemove.emplace(halfEdge->origin);
                    removeHalfEdge(halfEdge);
					isremove = true;
                }
                                else if (nbIntersections == 2)
                {
                    verticesToRemove.emplace(halfEdge->origin);
                    if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end())
                    {
                        halfEdge->origin = halfEdge->twin->destination;
                        halfEdge->destination = halfEdge->twin->origin;
                    }
                    else
                    {
                        halfEdge->origin = createVertex(intersections[0].point);
                        halfEdge->destination = createVertex(intersections[1].point);
                    }
                    if (outgoingHalfEdge != nullptr)
                        link2(box, outgoingHalfEdge, outgoingSide, halfEdge, intersections[0].id);
                    if (incomingHalfEdge == nullptr)
                    {
                       incomingHalfEdge = halfEdge;
                       incomingSide = intersections[0].id;
                    }
                    outgoingHalfEdge = halfEdge;
                    outgoingSide = intersections[1].id;
                    processedHalfEdges.emplace(halfEdge);
                }
                else
                    error = true;
            }
                        else if (inside && !nextInside)
            {
                if (nbIntersections == 1)
                {
                    if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end())
                        halfEdge->destination = halfEdge->twin->origin;
                    else
                        halfEdge->destination = createVertex(intersections[0].point);
                    outgoingHalfEdge = halfEdge;
                    outgoingSide = intersections[0].id;
                    processedHalfEdges.emplace(halfEdge);
                }
                else
                    error = true;
            }
                        else if (!inside && nextInside)
            {
                if (nbIntersections == 1)
                {
                    verticesToRemove.emplace(halfEdge->origin);
                    if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end())
                        halfEdge->origin = halfEdge->twin->destination;
                    else
                        halfEdge->origin = createVertex(intersections[0].point);
                    if (outgoingHalfEdge != nullptr)
                        link2(box, outgoingHalfEdge, outgoingSide, halfEdge, intersections[0].id);
                    if (incomingHalfEdge == nullptr)
                    {
                       incomingHalfEdge = halfEdge;
                       incomingSide = intersections[0].id;
                    }
                    processedHalfEdges.emplace(halfEdge);
                }
                else
                    error = true;
            }
			if (site.index < vaildsize && !isremove)
			{
				getAngle(halfEdge);
			}
            halfEdge = nextHalfEdge;
                        inside = nextInside;
        } while (halfEdge != site.face->outerComponent && halfEdge != nullptr);
                if (outerComponentDirty && incomingHalfEdge != nullptr)
            link2(box, outgoingHalfEdge, outgoingSide, incomingHalfEdge, incomingSide);
                if (outerComponentDirty)
            site.face->outerComponent = incomingHalfEdge;
    }
        for (auto& vertex : verticesToRemove)
        removeVertex(vertex);
        return !error;
}

std::vector<Vector2> VoronoiDiagram::getCentroidPos(int retainsize)
{
	std::vector<Vector2> pos;
	for (size_t i = 0; i < getNbSites() - retainsize; i++)
	{
		const VoronoiDiagram::Site* site = getSite(i);
				VoronoiDiagram::HalfEdge* halfEdge = site->face->outerComponent;
		VoronoiDiagram::HalfEdge* start = halfEdge;
		double x = 0,y = 0;
		int nbv = 0;
		while (true)
		{
			x += halfEdge->origin->point.x;
			y += halfEdge->origin->point.y;
			nbv++;
			halfEdge = halfEdge->next;
			if (halfEdge == start)
				break;
		}
		x /= nbv;
		y /= nbv;
		pos.push_back(Vector2(x, y));
	}
	for (size_t i = getNbSites() - retainsize; i < getNbSites(); i++)
	{
		const VoronoiDiagram::Site* site = getSite(i);
		pos.push_back(site->point);
	}
	return pos;
}
Vector2 getlineprojection(Vector2 P, Vector2 A, Vector2 B) {
	Vector2 v = B - A;
	return A + v * (v.dot(P - A) / v.dot(v));
}
std::vector<Vector2> VoronoiDiagram::getCfCVDPos(int retainsize, int &n)
{
	std::vector<Vector2> pos;
	std::vector<Vector2> tpos;
	std::vector<int> ids;
	n = 0;
	VoronoiDiagram::HalfEdge* boundBegin = nullptr;
	for (size_t i = 0; i < getNbSites() - retainsize; i++)
	{
		const VoronoiDiagram::Site* site = getSite(i);
		std::vector<VoronoiDiagram::HalfEdge*> angles;
				VoronoiDiagram::HalfEdge* halfEdge = site->face->outerComponent;
		VoronoiDiagram::HalfEdge* start = halfEdge;
		double x = 0, y = 0;
		int nbv = 0;
		while (true)
		{
			if (boundBegin == nullptr && halfEdge->twin == nullptr)
			{
				boundBegin = halfEdge;
			}
			x += halfEdge->origin->point.x;
			y += halfEdge->origin->point.y;
			nbv++;
			halfEdge = halfEdge->next;
			if (halfEdge == start)
				break;
		}
		x /= nbv;
		y /= nbv;
		tpos.push_back(Vector2(x, y));
	}
		if (boundBegin != nullptr)
	{
		std::vector<VoronoiDiagram::HalfEdge*> bounds;
		VoronoiDiagram::HalfEdge* corner = nullptr;
		VoronoiDiagram::HalfEdge* start = boundBegin;
		VoronoiDiagram::HalfEdge* halfEdge = boundBegin;
		while (true)
		{
						double x;
			double y;
			
			
			Vector2 tp;
												tp = (halfEdge->origin->point + halfEdge->destination->point) / 2;
			x = tp.x;
			y = tp.y;
						if (halfEdge->incidentFace->site->index < tpos.size())
			{
				ids.push_back(halfEdge->incidentFace->site->index);
				tpos[halfEdge->incidentFace->site->index] = Vector2(x, y);
			}

			if (halfEdge->next->twin != nullptr)
			{
				halfEdge = halfEdge->next->twin->next;
				bounds.push_back(halfEdge);
			}
			else
			{
								halfEdge = halfEdge->next;
				bounds.push_back(nullptr);
			}
			if (halfEdge == start)
				break;
		}
	}
		for (size_t i = 0; i < getNbSites() - retainsize; i++)
	{
		if (std::find(ids.begin(), ids.end(), i) == ids.end())
			pos.insert(pos.begin(), tpos[i]);
		else
		{
			pos.push_back(tpos[i]);
			n++;
		}

	}
	for (size_t i = getNbSites() - retainsize; i < getNbSites(); i++)
		pos.push_back(getSite(i)->point);
	
	return pos;
}

void VoronoiDiagram::getBoundpos(VoronoiDiagram &parentDiagram, VoronoiDiagram &childrenDiagram)
{
		VoronoiDiagram::HalfEdge* start = nullptr;
	for (int i = mSites.size() - 1; i >= 0; i--)
	{
		Site site = mSites[i];
		VoronoiDiagram::HalfEdge* halfEdge = site.face->outerComponent;
		VoronoiDiagram::HalfEdge* end = halfEdge;
		while (true)
		{
			if (halfEdge->twin == nullptr)
			{
								start = halfEdge;
				i = -1;
				break;
			}
			halfEdge = halfEdge->next;
			if (halfEdge == end)
				break;
		}
	}
		VoronoiDiagram::HalfEdge* end = start;
	start = start->next->twin->prev;
	while (start != end)
	{

	}
	if (start->next->twin->prev == nullptr)
	{
		VoronoiDiagram::HalfEdge* halfEdge = start->next->twin->prev;
	}
}

void VoronoiDiagram::getAngle(VoronoiDiagram::HalfEdge* halfEdge)
{
	Vector2 A = halfEdge->incidentFace->site->point;
	Vector2 B = halfEdge->origin->point;
	Vector2 C = halfEdge->destination->point;
	double x1 = B.x;
	double x2 = C.x;
	double x3 = A.x;
	double y1 = B.y;
	double y2 = C.y;
	double y3 = A.y;
	double theta = atan2(x1 - x3, y1 - y3) - atan2(x2 - x3, y2 - y3);
	if (theta > std::_Pi)
		theta -= 2 * std::_Pi;
	if (theta < -std::_Pi)
		theta += 2 * std::_Pi;

	theta = abs(theta * 180.0 / std::_Pi);
	halfEdge->angle = theta;
	
}

VoronoiDiagram::Vertex* VoronoiDiagram::createVertex(Vector2 point)
{
    mVertices.emplace_back();
    mVertices.back().point = point;
    mVertices.back().it = std::prev(mVertices.end());
    return &mVertices.back();
}
VoronoiDiagram::Vertex* VoronoiDiagram::createCorner2(Box box, int side)
{
	return createVertex(box.corpos[side]);
}
VoronoiDiagram::Vertex* VoronoiDiagram::createCorner(Box box, Box::Side side)
{
    switch (side)
    {
        case Box::Side::LEFT:
            return createVertex(Vector2(box.left, box.top));
        case Box::Side::BOTTOM:
            return createVertex(Vector2(box.left, box.bottom));
        case Box::Side::RIGHT:
            return createVertex(Vector2(box.right, box.bottom));
        case Box::Side::TOP:
            return createVertex(Vector2(box.right, box.top));
        default:
            return nullptr;
    }
}

VoronoiDiagram::HalfEdge* VoronoiDiagram::createHalfEdge(Face* face)
{
    mHalfEdges.emplace_back();
    mHalfEdges.back().incidentFace = face;
    mHalfEdges.back().it = std::prev(mHalfEdges.end());
    if(face->outerComponent == nullptr)
        face->outerComponent = &mHalfEdges.back();
    return &mHalfEdges.back();
}

void VoronoiDiagram::link(Box box, HalfEdge* start, Box::Side startSide, HalfEdge* end, Box::Side endSide)
{
	HalfEdge* halfEdge = start;
	int side = static_cast<int>(startSide);
	while (side != static_cast<int>(endSide))
	{
		side = (side + 1) % 4;
		halfEdge->next = createHalfEdge(start->incidentFace);
		halfEdge->next->prev = halfEdge;
		halfEdge->next->origin = halfEdge->destination;
		halfEdge->next->destination = createCorner(box, static_cast<Box::Side>(side));
		halfEdge = halfEdge->next;
	}
	halfEdge->next = createHalfEdge(start->incidentFace);
	halfEdge->next->prev = halfEdge;
	end->prev = halfEdge->next;
	halfEdge->next->next = end;
	halfEdge->next->origin = halfEdge->destination;
	halfEdge->next->destination = end->origin;
}
void VoronoiDiagram::link2(Box box, HalfEdge* start, int startSide, HalfEdge* end, int endSide)
{
	HalfEdge* halfEdge = start;
	int side = startSide;
	while (side != endSide)
	{
		side = (side + 1) % box.corpos.size();
		halfEdge->next = createHalfEdge(start->incidentFace);
		halfEdge->next->prev = halfEdge;
		halfEdge->next->origin = halfEdge->destination;
		halfEdge->next->destination = createCorner2(box, side);
		halfEdge = halfEdge->next;
	}
	halfEdge->next = createHalfEdge(start->incidentFace);
	halfEdge->next->prev = halfEdge;
	end->prev = halfEdge->next;
	halfEdge->next->next = end;
	halfEdge->next->origin = halfEdge->destination;
	halfEdge->next->destination = end->origin;
	getAngle(halfEdge->next);
}
void VoronoiDiagram::removeVertex(Vertex* vertex)
{
    mVertices.erase(vertex->it);
}

void VoronoiDiagram::removeHalfEdge(HalfEdge* halfEdge)
{
    mHalfEdges.erase(halfEdge->it);
}

