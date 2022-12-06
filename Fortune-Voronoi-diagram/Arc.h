

#pragma once

#include "VoronoiDiagram.h"

class Event;
struct Arc
{
    enum class Color{RED, BLACK};

        Arc* parent;
    Arc* left;
    Arc* right;
        VoronoiDiagram::Site* site;
    VoronoiDiagram::HalfEdge* leftHalfEdge;
    VoronoiDiagram::HalfEdge* rightHalfEdge;
    Event* event;
        Arc* prev;
    Arc* next;
        Color color;
};

