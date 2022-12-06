

#pragma once

#include <ostream>


class Vector2;
Vector2 operator-(Vector2 lhs, const Vector2& rhs);


class Vector2
{
public:
    double x;
    double y;

    Vector2(double x = 0.0, double y = 0.0);
    
    
    Vector2 operator-() const;
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(double t);
    
        
    Vector2 getOrthogonal() const;
    double dot(const Vector2& other) const;
    double getNorm() const;
    double getDistance(const Vector2& other) const;
    double getDet(const Vector2& other) const;
};


Vector2 operator+(Vector2 lhs, const Vector2& rhs);
Vector2 operator-(Vector2 lhs, const Vector2& rhs);
Vector2 operator*(double t, Vector2 vec);
Vector2 operator*(Vector2 vec, double t);
Vector2 operator/(Vector2 vec, double t);
std::ostream& operator<<(std::ostream& os, const Vector2& vec);

