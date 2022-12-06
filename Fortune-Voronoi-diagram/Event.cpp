

#include "Event.h"

Event::Event(VoronoiDiagram::Site* site) : type(Type::SITE), y(site->point.y), index(-1), site(site)
{

}

Event::Event(VoronoiDiagram::Circle* circle) : type(Type::CIRCLE), y(circle->y), index(-1), point(circle->point), arc(circle->arc)
{


}
bool operator<(const Event& lhs, const Event& rhs)
{
	bool is = false;
		if (lhs.y < rhs.y)
		is = true;
    return is;
}

std::ostream& operator<<(std::ostream& os, const Event& event)
{
    if(event.type == Event::Type::SITE)
        os << "S(" << event.site->index << ", " << event.y << ")";
    else
        os << "C(" << event.arc << ", " << event.y << ", " << event.point << ")";
    return os;
}

