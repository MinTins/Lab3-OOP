

#include "Box.h"
#include <iostream>
int Box::contains(const Vector2& point)
{
	int presize = 0;
	int nextsize = 1;
	int corsize = corpos.size();
	int ret1 = 0;
	int ret2 = 0;
	while (presize < corsize)
	{
		Vector2 prepos = corpos[presize];
		Vector2 nextpos = corpos[nextsize];
				Vector2 pa = prepos - point;
		Vector2 pb = nextpos - point;
		double ab = pb.y*pa.x - pb.x*pa.y;
		if (ab < 0)
			ret1++;
		else if (ab > 0)
			ret2++;
		else
		{
			if (point.x <= std::max(prepos.x, nextpos.x) && point.x >= std::min(prepos.x, nextpos.x) &&
				point.y <= std::max(prepos.y, nextpos.y) && point.y >= std::min(prepos.y, nextpos.y))
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
			
		presize++;
		nextsize++;
		nextsize %= corsize;
	}
	if (ret1 == 0 || ret2 == 0)
		return 1;
	else
		return -1;
}

Box::Intersection Box::getFirstIntersection(const Vector2& origin, const Vector2& direction) const
{
        Intersection intersection;
    double t = std::numeric_limits<double>::infinity();
    if (direction.x > 0.0)
    {
        t = (right - origin.x) / direction.x;
        intersection.side = Side::RIGHT;
        intersection.point = origin + t * direction;
    }
    else if (direction.x < 0.0)
    {
        t = (left - origin.x) / direction.x;
        intersection.side = Side::LEFT;
        intersection.point = origin + t * direction;
    }
    if (direction.y > 0.0)
    {
        double newT = (top - origin.y) / direction.y;
        if (newT < t)
        {
            intersection.side = Side::TOP;
            intersection.point = origin + newT * direction;
        }
    }
    else if (direction.y < 0.0)
    {
        double newT = (bottom - origin.y) / direction.y;
        if (newT < t)
        {
            intersection.side = Side::BOTTOM;
            intersection.point = origin + newT * direction;
        }
    }
    return intersection;
}
void Box::GetLinePara(Line &l)
{
	l.a = l.p1.y - l.p2.y;
	l.b = l.p2.x - l.p1.x;
	l.c = l.p1.x*l.p2.y - l.p1.y*l.p2.x;
}
Vector2 Box::getCrossPoint(Line &l1, Line &l2)
{
	GetLinePara(l1);
	GetLinePara(l2);
	double D = l1.a*l2.b - l2.a*l1.b;
	Vector2 p(0.0,0.0);
	p.x = (l1.b*l2.c - l2.b*l1.c) / D;
	p.y = (l1.c*l2.a - l2.c*l1.a) / D;
	return p;
}
bool Box::IsRectCross(const Vector2 &p1, const Vector2 &p2, const Vector2 &q1, const Vector2 &q2)
{
	bool ret = std::min(p1.x, p2.x) <= std::max(q1.x, q2.x) &&
		std::min(q1.x, q2.x) <= std::max(p1.x, p2.x) &&
		std::min(p1.y, p2.y) <= std::max(q1.y, q2.y) &&
		std::min(q1.y, q2.y) <= std::max(p1.y, p2.y);
	return ret;
}
bool Box::IsLineSegmentCross(const Vector2 &pFirst1, const Vector2 &pFirst2, const Vector2 &pSecond1, const Vector2 &pSecond2)
{
	Vector2 a = pFirst1;
	Vector2 b = pFirst2;
	Vector2 c = pSecond1;
	Vector2 d = pSecond2;
	if (!(std::min(a.x, b.x) <= std::max(c.x, d.x) && std::min(c.y, d.y) <= std::max(a.y, b.y) && std::min(c.x, d.x) <= std::max(a.x, b.x) && std::min(a.y, b.y) <= std::max(c.y, d.y)))
		return false;
	double u, v, w, z;
	u = (c.x - a.x)*(b.y - a.y) - (b.x - a.x)*(c.y - a.y);
	v = (d.x - a.x)*(b.y - a.y) - (b.x - a.x)*(d.y - a.y);
	w = (a.x - c.x)*(d.y - c.y) - (d.x - c.x)*(a.y - c.y);
	z = (b.x - c.x)*(d.y - c.y) - (d.x - c.x)*(b.y - c.y);
	return (u*v <= 0.00000001 && w*z <= 0.00000001);

	long line1, line2;
	double t1 = pFirst1.x * (pSecond1.y - pFirst2.y) +
		pFirst2.x * (pFirst1.y - pSecond1.y) +
		pSecond1.x * (pFirst2.y - pFirst1.y);
	double t2 = pFirst1.x * (pSecond2.y - pFirst2.y) +
		pFirst2.x * (pFirst1.y - pSecond2.y) +
		pSecond2.x * (pFirst2.y - pFirst1.y);
	line1 = pFirst1.x * (pSecond1.y - pFirst2.y) +
		pFirst2.x * (pFirst1.y - pSecond1.y) +
		pSecond1.x * (pFirst2.y - pFirst1.y);
	line2 = pFirst1.x * (pSecond2.y - pFirst2.y) +
		pFirst2.x * (pFirst1.y - pSecond2.y) +
		pSecond2.x * (pFirst2.y - pFirst1.y);
	if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0))
		return false;

	line1 = pSecond1.x * (pFirst1.y - pSecond2.y) +
		pSecond2.x * (pSecond1.y - pFirst1.y) +
		pFirst1.x * (pSecond2.y - pSecond1.y);
	line2 = pSecond1.x * (pFirst2.y - pSecond2.y) +
		pSecond2.x * (pSecond1.y - pFirst2.y) +
		pFirst2.x * (pSecond2.y - pSecond1.y);
	if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0))
		return false;
	return true;
}
bool Box::getCrossPoint(const Vector2 &p1, const Vector2 &p2, const Vector2 &q1, const Vector2 &q2, Vector2& v, double& d)
{
	if (IsRectCross(p1, p2, q1, q2))
	{
		if (IsLineSegmentCross(p1, p2, q1, q2))
		{
			Line l1, l2;
			l1.p1.x = p1.x;
			l1.p1.y = p1.y;
			l1.p2.x = p2.x;
			l1.p2.y = p2.y;
			l2.p1.x = q1.x;
			l2.p1.y = q1.y;
			l2.p2.x = q2.x;
			l2.p2.y = q2.y;
			Vector2 p = getCrossPoint(l1, l2);
			if (p.x == 0.0 && p.y == 0.0)
			{
				return false;
			}
			v.x = p.x;
			v.y = p.y;

						d = std::sqrt(std::pow(q1.x - v.x, 2) + std::pow(q1.y - v.y, 2));
			return true;
		}
	}
	return false;
}
int Box::getIntersections(const Vector2& origin, const Vector2& destination, std::array<Intersection, 2>& intersections)
{
        Vector2 direction = destination - origin;
    std::array<double, 2> t;
    std::size_t i = 0; 	
	int presize = 0;
	int nextsize = 1;
	int corsize = corpos.size();
	while (presize < corsize)
	{
		Vector2 prepos = corpos[presize];
		Vector2 nextpos = corpos[nextsize];
		if (getCrossPoint(prepos, nextpos, origin, destination, intersections[i].point, t[i]))
		{
			if ((std::abs(origin.y - destination.y) <= EPSILON && std::abs(prepos.y - nextpos.y) <= EPSILON) ||
				(std::abs(origin.x - destination.x) <= EPSILON && std::abs(prepos.x - nextpos.x) <= EPSILON) ||
				((std::abs(intersections[i].point.x - origin.x) < 0.0000001 && std::abs(intersections[i].point.y - origin.y) < 0.0000001) ||
				(std::abs(intersections[i].point.x - destination.x) < 0.0000001 && std::abs(intersections[i].point.y - destination.y) < 0.0000001)))
			{
				int a = 0;
			}
			else
			{
				intersections[i++].id = presize;
								if (i == 2)
				{
					if (t[0] > t[1])
					{
						std::swap(intersections[0], intersections[1]);
						int a = 0;
					}
					break;
				}
			}
					}
		presize++;
		nextsize++;
		nextsize %= corsize;
	}
	return i;
	
        if (origin.x < left - EPSILON || destination.x < left - EPSILON)
    {   
        t[i] = (left - origin.x) / direction.x;
        if (t[i] > EPSILON && t[i] < 1.0 - EPSILON)  
        {
            intersections[i].side = Side::LEFT;
            intersections[i].point = origin + t[i] * direction;
            if (intersections[i].point.y >= bottom  - EPSILON && intersections[i].point.y <= top + EPSILON)
                ++i;
        }
    }
        if (origin.x > right + EPSILON || destination.x > right + EPSILON)
    {   
        t[i] = (right - origin.x) / direction.x;
        if (t[i] > EPSILON && t[i] < 1.0 - EPSILON)  
        {
            intersections[i].side = Side::RIGHT;
            intersections[i].point = origin + t[i] * direction;
            if (intersections[i].point.y >= bottom - EPSILON && intersections[i].point.y <= top + EPSILON)
                ++i;
        }
    }
        if (origin.y < bottom - EPSILON || destination.y < bottom - EPSILON)
    {   
        t[i] = (bottom - origin.y) / direction.y;
        if (i < 2 && t[i] > EPSILON && t[i] < 1.0 - EPSILON)  
        {
            intersections[i].side = Side::BOTTOM;
            intersections[i].point = origin + t[i] * direction;
            if (intersections[i].point.x >= left  - EPSILON && intersections[i].point.x <= right + EPSILON)
                ++i;
        }
    }
        if (origin.y > top + EPSILON || destination.y > top + EPSILON)
    {   
        t[i] = (top - origin.y) / direction.y;
        if (i < 2 && t[i] > EPSILON && t[i] < 1.0 - EPSILON)  
        {
            intersections[i].side = Side::TOP;
            intersections[i].point = origin + t[i] * direction;
            if (intersections[i].point.x >= left - EPSILON && intersections[i].point.x <= right + EPSILON)
                ++i;
        }
    }
    	
    if (i == 2 && t[0] > t[1])
        std::swap(intersections[0], intersections[1]);
    return i;
}
