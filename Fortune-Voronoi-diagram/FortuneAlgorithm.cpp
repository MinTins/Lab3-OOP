

#include "FortuneAlgorithm.h"
#include "Arc.h"
#include "Event.h"
#include <iostream>
FortuneAlgorithm::FortuneAlgorithm(std::vector<Vector2> points) : mDiagram(std::move(points))
{

}

FortuneAlgorithm::~FortuneAlgorithm() = default;

void FortuneAlgorithm::construct()
{
        for (std::size_t i = 0; i < mDiagram.getNbSites(); ++i)
        mEvents.push(std::make_unique<Event>(mDiagram.getSite(i)));

        while (!mEvents.isEmpty())
    {
        std::unique_ptr<Event> event = mEvents.pop();
        mBeachlineY = event->y;
		        if(event->type == Event::Type::SITE)
            handleSiteEvent(event.get());
        else
            handleCircleEvent(event.get());
    }
}

VoronoiDiagram FortuneAlgorithm::getDiagram()
{
    return std::move(mDiagram);
}


void FortuneAlgorithm::handleSiteEvent(Event* event)
{
    VoronoiDiagram::Site* site = event->site;
        if (mBeachline.isEmpty())
    {
        mBeachline.setRoot(mBeachline.createArc(site));
        return;
    }
        Arc* arcToBreak = mBeachline.locateArcAbove(site->point, mBeachlineY);
    deleteEvent(arcToBreak);
        Arc* middleArc = breakArc(arcToBreak, site);
	
    Arc* leftArc = middleArc->prev; 
    Arc* rightArc = middleArc->next;
        addEdge(leftArc, middleArc);
    middleArc->rightHalfEdge = middleArc->leftHalfEdge;
    rightArc->leftHalfEdge = leftArc->rightHalfEdge;
            if (!mBeachline.isNil(leftArc->prev))
        addEvent(leftArc->prev, leftArc, middleArc);
        if (!mBeachline.isNil(rightArc->next))
        addEvent(middleArc, rightArc, rightArc->next);
}

void FortuneAlgorithm::handleCircleEvent(Event* event)
{
    Vector2 point = event->point;
    Arc* arc = event->arc;
        VoronoiDiagram::Vertex* vertex = mDiagram.createVertex(point);
        Arc* leftArc = arc->prev;
    Arc* rightArc = arc->next;
    deleteEvent(leftArc);
    deleteEvent(rightArc);
        removeArc(arc, vertex);
            if (!mBeachline.isNil(leftArc->prev))
        addEvent(leftArc->prev, leftArc, rightArc);
        if (!mBeachline.isNil(rightArc->next))
        addEvent(leftArc, rightArc, rightArc->next);
}

Arc* FortuneAlgorithm::breakArc(Arc* arc, VoronoiDiagram::Site* site)
{
        Arc* middleArc = mBeachline.createArc(site);
    Arc* leftArc = mBeachline.createArc(arc->site);
    leftArc->leftHalfEdge = arc->leftHalfEdge;
    Arc* rightArc = mBeachline.createArc(arc->site);
    rightArc->rightHalfEdge = arc->rightHalfEdge;
        mBeachline.replace(arc, middleArc);
	mBeachline.insertBefore(middleArc, leftArc);
	mBeachline.insertAfter(middleArc, rightArc);
        delete arc;
        return middleArc;
}

void FortuneAlgorithm::removeArc(Arc* arc, VoronoiDiagram::Vertex* vertex)
{
        setDestination(arc->prev, arc, vertex);
    setDestination(arc, arc->next, vertex);
        arc->leftHalfEdge->next = arc->rightHalfEdge;
    arc->rightHalfEdge->prev = arc->leftHalfEdge;
        mBeachline.remove(arc);
        VoronoiDiagram::HalfEdge* prevHalfEdge = arc->prev->rightHalfEdge;
    VoronoiDiagram::HalfEdge* nextHalfEdge = arc->next->leftHalfEdge;
    addEdge(arc->prev, arc->next);
    setOrigin(arc->prev, arc->next, vertex);
    setPrevHalfEdge(arc->prev->rightHalfEdge, prevHalfEdge);
    setPrevHalfEdge(nextHalfEdge, arc->next->leftHalfEdge);
        delete arc;
}

bool FortuneAlgorithm::isMovingRight(const Arc* left, const Arc* right) const
{
    return left->site->point.y <= right->site->point.y;
}

double FortuneAlgorithm::getInitialX(const Arc* left, const Arc* right, bool movingRight) const
{
    return movingRight ? left->site->point.x : right->site->point.x;
}

void FortuneAlgorithm::addEdge(Arc* left, Arc* right)
{
        left->rightHalfEdge = mDiagram.createHalfEdge(left->site->face);
    right->leftHalfEdge = mDiagram.createHalfEdge(right->site->face);
        left->rightHalfEdge->twin = right->leftHalfEdge;
    right->leftHalfEdge->twin = left->rightHalfEdge;
}

void FortuneAlgorithm::setOrigin(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex)
{
    left->rightHalfEdge->destination = vertex;
    right->leftHalfEdge->origin = vertex;
}

void FortuneAlgorithm::setDestination(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex)
{
    left->rightHalfEdge->origin = vertex;
    right->leftHalfEdge->destination = vertex;
}

void FortuneAlgorithm::setPrevHalfEdge(VoronoiDiagram::HalfEdge* prev, VoronoiDiagram::HalfEdge* next)
{
    prev->next = next;
    next->prev = prev;
}

void FortuneAlgorithm::addEvent(Arc* left, Arc* middle, Arc* right)
{
    double y;
    Vector2 convergencePoint = computeConvergencePoint(left->site->point, middle->site->point, right->site->point, y);
	bool isToofar = true;	if (std::abs(convergencePoint.x) > 100000000 || std::abs(convergencePoint.y) > 100000000)
	{
		isToofar = false;
	}
    bool isBelow = y <= mBeachlineY;
    bool leftBreakpointMovingRight = isMovingRight(left, middle);
    bool rightBreakpointMovingRight = isMovingRight(middle, right);
    double leftInitialX = getInitialX(left, middle, leftBreakpointMovingRight);
    double rightInitialX = getInitialX(middle, right, rightBreakpointMovingRight);
    bool isValid =
        ((leftBreakpointMovingRight && leftInitialX < convergencePoint.x) ||
        (!leftBreakpointMovingRight && leftInitialX > convergencePoint.x)) 
		&&
        ((rightBreakpointMovingRight && rightInitialX < convergencePoint.x) ||
        (!rightBreakpointMovingRight && rightInitialX > convergencePoint.x));
    if (isValid && isBelow && isToofar)
    {
        VoronoiDiagram::Circle *circle = new VoronoiDiagram::Circle();
		circle->y = y;
		circle->point = convergencePoint;
		circle->arc = middle;
        std::unique_ptr<Event> event = std::make_unique<Event>(circle);
        middle->event = event.get();
        mEvents.push(std::move(event));
    }
}

void FortuneAlgorithm::deleteEvent(Arc* arc)
{
    if (arc->event != nullptr)
    {
        mEvents.remove(arc->event->index);
        arc->event = nullptr;
    }
}

Vector2 FortuneAlgorithm::computeConvergencePoint(const Vector2& point1, const Vector2& point2, const Vector2& point3, double& y) const
{
    Vector2 v1 = (point1 - point2).getOrthogonal();
    Vector2 v2 = (point2 - point3).getOrthogonal();
    Vector2 delta = 0.5 * (point3 - point1);
    double t = delta.getDet(v2) / v1.getDet(v2);
    Vector2 center = 0.5 * (point1 + point2) + t * v1;
    double r = center.getDistance(point1);
    y = center.y - r;
    return center;
}


#include <list>
#include <unordered_map>

bool FortuneAlgorithm::bound(Box box)
{
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
	box.left -= 10;
	box.bottom -= 10;
	box.right += 10;
	box.top += 10;
        std::list<LinkedVertex> linkedVertices;
    std::unordered_map<std::size_t, std::array<LinkedVertex*, 8>> vertices(mDiagram.getNbSites());
    if (!mBeachline.isEmpty())
    {
        Arc* leftArc = mBeachline.getLeftmostArc();
        Arc* rightArc = leftArc->next;
        while (!mBeachline.isNil(rightArc))
        {
                        Vector2 direction = (leftArc->site->point - rightArc->site->point).getOrthogonal();
            Vector2 origin = (leftArc->site->point + rightArc->site->point) * 0.5f;
                        Box::Intersection intersection = box.getFirstIntersection(origin, direction);
                        VoronoiDiagram::Vertex* vertex = mDiagram.createVertex(intersection.point);
            setDestination(leftArc, rightArc, vertex);
                        if (vertices.find(leftArc->site->index) == vertices.end()) 
                vertices[leftArc->site->index].fill(nullptr); 
            if (vertices.find(rightArc->site->index) == vertices.end()) 
                vertices[rightArc->site->index].fill(nullptr); 
                        linkedVertices.emplace_back(LinkedVertex{nullptr, vertex, leftArc->rightHalfEdge});
            vertices[leftArc->site->index][2 * static_cast<int>(intersection.side) + 1] = &linkedVertices.back();
            linkedVertices.emplace_back(LinkedVertex{rightArc->leftHalfEdge, vertex, nullptr});
            vertices[rightArc->site->index][2 * static_cast<int>(intersection.side)] = &linkedVertices.back();
                        leftArc = rightArc;
            rightArc = rightArc->next;
        }
    }
        for (auto& kv : vertices)
    {
        auto& cellVertices = kv.second;
                for (std::size_t i = 0; i < 5; ++i)
        {
            std::size_t side = i % 4;
            std::size_t nextSide = (side + 1) % 4;
                        if (cellVertices[2 * side] == nullptr && cellVertices[2 * side + 1] != nullptr)
            {
                std::size_t prevSide = (side + 3) % 4;
                VoronoiDiagram::Vertex* corner = mDiagram.createCorner(box, static_cast<Box::Side>(side));
                linkedVertices.emplace_back(LinkedVertex{nullptr, corner, nullptr});
                cellVertices[2 * prevSide + 1] = &linkedVertices.back();
                cellVertices[2 * side] = &linkedVertices.back();
            }
                        else if (cellVertices[2 * side] != nullptr && cellVertices[2 * side + 1] == nullptr)
            {
                VoronoiDiagram::Vertex* corner = mDiagram.createCorner(box, static_cast<Box::Side>(nextSide));
                linkedVertices.emplace_back(LinkedVertex{nullptr, corner, nullptr});
                cellVertices[2 * side + 1] = &linkedVertices.back();
                cellVertices[2 * nextSide] = &linkedVertices.back();
            }
        }
    }
        for (auto& kv : vertices)
    {
        std::size_t i = kv.first;
        auto& cellVertices = kv.second;
        for (std::size_t side = 0; side < 4; ++side)
        {
            if (cellVertices[2 * side] != nullptr)
            {
                                VoronoiDiagram::HalfEdge* halfEdge = mDiagram.createHalfEdge(mDiagram.getFace(i));
                halfEdge->origin = cellVertices[2 * side]->vertex;
                halfEdge->destination = cellVertices[2 * side + 1]->vertex;
                cellVertices[2 * side]->nextHalfEdge = halfEdge;
                halfEdge->prev = cellVertices[2 * side]->prevHalfEdge;
                if (cellVertices[2 * side]->prevHalfEdge != nullptr)
                    cellVertices[2 * side]->prevHalfEdge->next = halfEdge;
                cellVertices[2 * side + 1]->prevHalfEdge = halfEdge;
                halfEdge->next = cellVertices[2 * side + 1]->nextHalfEdge;
                if (cellVertices[2 * side + 1]->nextHalfEdge != nullptr)
                    cellVertices[2 * side + 1]->nextHalfEdge->prev = halfEdge;
            }
        }
    }
    return true; }

