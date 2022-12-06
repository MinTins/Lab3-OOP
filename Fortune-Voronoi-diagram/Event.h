

#pragma once

#include "Vector2.h"
#include "VoronoiDiagram.h"

class Arc;

class Event
{
public:
    enum class Type{SITE, CIRCLE};

        Event(VoronoiDiagram::Site* site);
		Event(VoronoiDiagram::Circle* circle);

    const Type type;
    double y;
    int index;
        VoronoiDiagram::Site* site;
        Vector2 point;
    Arc* arc;

};

bool operator<(const Event& lhs, const Event& rhs);
std::ostream& operator<<(std::ostream& os, const Event& event);

