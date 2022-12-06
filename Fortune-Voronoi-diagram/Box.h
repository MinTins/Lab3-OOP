

#pragma once

#include <array>
#include <limits>
#include "Vector2.h"
#include <vector>
class Box
{
public:
        enum class Side : int {LEFT, BOTTOM, RIGHT, TOP};

    struct Intersection
    {
		int id;
        Side side;
        Vector2 point;
    };
	struct Line
	{
		Vector2 p1, p2;
		double a, b, c;
	};
	std::vector<Vector2> corpos;
    double left;
    double bottom;
    double right;
    double top;

    int contains(const Vector2& point);
    Intersection getFirstIntersection(const Vector2& origin, const Vector2& direction) const;     int getIntersections(const Vector2& origin, const Vector2& destination, std::array<Intersection, 2>& intersections) ; 

	bool IsLineSegmentCross(const Vector2 &pFirst1, const Vector2 &pFirst2, const Vector2 &pSecond1, const Vector2 &pSecond2);
	bool getCrossPoint(const Vector2 &p1, const Vector2 &p2, const Vector2 &q1, const Vector2 &q2, Vector2& v, double& d);
	bool IsRectCross(const Vector2 &p1, const Vector2 &p2, const Vector2 &q1, const Vector2 &q2);
	Vector2 getCrossPoint(Line &l1, Line &l2);
	void GetLinePara(Line &l);
private:
    static constexpr double EPSILON = std::numeric_limits<double>::epsilon();
};

