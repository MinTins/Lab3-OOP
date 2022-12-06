

#pragma once

#include <vector>
#include <list>
#include "Box.h"

class FortuneAlgorithm;
class Arc;
class Boundp;
class VoronoiDiagram
{
public:
    struct HalfEdge;
    struct Face;
	
    struct Site
    {
        std::size_t index;
        Vector2 point;
        Face* face;
    };
	
	struct Circle
	{
		double y;
		Vector2 point;
		Arc* arc;
	};
    struct Vertex
    {
        Vector2 point;

    private:
        friend VoronoiDiagram;
        std::list<Vertex>::iterator it;
    };
	struct ParentHalfEdge
	{
		bool isMarginEdge = false; 		HalfEdge* parent = nullptr;		std::size_t edgeid = -1;	};
	struct ChildrenHalfEdge
	{
		bool isMarginEdge = false; 		std::vector<HalfEdge*> childrens;	};
    struct HalfEdge
    {
        Vertex* origin = nullptr;
        Vertex* destination = nullptr;
        HalfEdge* twin = nullptr;
        Face* incidentFace;
        HalfEdge* prev = nullptr;
        HalfEdge* next = nullptr;
		double angle; 		ParentHalfEdge parentEdge; 		ChildrenHalfEdge chldrensEdge;		bool isbound = false;    private:
        friend VoronoiDiagram;
        std::list<HalfEdge>::iterator it;
    };
    struct Face
    {
        Site* site;
        HalfEdge* outerComponent;
    };

    VoronoiDiagram(const std::vector<Vector2>& points);
	VoronoiDiagram();
        VoronoiDiagram(const VoronoiDiagram&) = delete;
    VoronoiDiagram& operator=(const VoronoiDiagram&) = delete;

        VoronoiDiagram(VoronoiDiagram&&) = default;
    VoronoiDiagram& operator=(VoronoiDiagram&&) = default;

        Site* getSite(std::size_t i);
    std::size_t getNbSites() const;
    Face* getFace(std::size_t i);
    const std::list<Vertex>& getVertices() const;
    const std::list<HalfEdge>& getHalfEdges() const;

        bool intersect(Box box);

	std::vector<Vector2> getCentroidPos(int retainsize);
	std::vector<Vector2> getCfCVDPos(int retainsize, int &n);
	void getBoundpos(VoronoiDiagram &parentDiagram, VoronoiDiagram &childrenDiagram);
	void getAngle(VoronoiDiagram::HalfEdge* halfEdge);
	std::vector<Vector2> mBoundSitesid;
	VoronoiDiagram::HalfEdge* mboundBegin = nullptr;
private:
    std::vector<Site> mSites;
    std::vector<Face> mFaces;
    std::list<Vertex> mVertices;
	std::list<HalfEdge> mHalfEdges;
	

        friend FortuneAlgorithm;

    Vertex* createVertex(Vector2 point);
	Vertex* createCorner(Box box, Box::Side side);
	Vertex* createCorner2(Box box, int side);
    HalfEdge* createHalfEdge(Face* face);

    	void link(Box box, HalfEdge* start, Box::Side startSide, HalfEdge* end, Box::Side endSide);
	void link2(Box box, HalfEdge* start, int startSide, HalfEdge* end, int endSide);
    void removeVertex(Vertex* vertex);
    void removeHalfEdge(HalfEdge* halfEdge);
};
