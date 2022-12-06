

#pragma once

#include "Vector2.h"
#include "VoronoiDiagram.h"

class Arc;

class Beachline
{
public:
    Beachline();
    ~Beachline();

        Beachline(const Beachline&) = delete;
    Beachline& operator=(const Beachline&) = delete;
    Beachline(Beachline&&) = delete;
    Beachline& operator=(Beachline&&) = delete;

    Arc* createArc(VoronoiDiagram::Site* site);
    
    bool isEmpty();
    bool isNil(Arc* x);
	void setRoot(Arc* x);
	Arc* getRoot() { return mRoot; };
    Arc* getLeftmostArc();

    Arc* locateArcAbove(const Vector2& point, double l);
    void insertBefore(Arc* x, Arc* y);
    void insertAfter(Arc* x, Arc* y);
    void replace(Arc* x, Arc* y);
    void remove(Arc* z);

    std::ostream& print(std::ostream& os);

private:
    Arc* mNil;
    Arc* mRoot;

        Arc* minimum(Arc* x);
    void transplant(Arc* u, Arc* v); 

        void insertFixup(Arc* z);
    void removeFixup(Arc* x);

        void leftRotate(Arc* x);
    void rightRotate(Arc* y);

    double computeBreakpoint(const Vector2& point1, const Vector2& point2, double l);

    void free(Arc* x);

    std::ostream& printArc(std::ostream& os, const Arc* arc, std::string tabs = "");
};

std::ostream& operator<<(std::ostream& os, Beachline& beachline);
