#include<cfloat>
#include<cmath>

#include "mathUtils.hpp"
#include "config.hpp"

std::mt19937 gen;

sf::Int32 randColor()
{
    return gen();
}
sf::Int32 randOpaqueColor()
{
    return gen() | 0xff;
}

float normalizeAngle(float x)
{
    float a = std::fmod(x + CONST::PI, 2.0f * CONST::PI);
    if(a <= 0.0f)
        a += 2.0f * CONST::PI;
    return a - CONST::PI;
}

float dis2(sf::Vector2f a, sf::Vector2f b)
{
    return std::hypot(a.x - b.x, a.y - b.y);
}

float cross(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.y - a.y * b.x;
}
float dot(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.x + a.y * b.y;
}
float length(sf::Vector2f a)
{
    return std::hypot(a.x, a.y);
}
sf::Vector2f normalizeVec(sf::Vector2f a, float desiredLength)
{
    return a / length(a) * desiredLength;
}
sf::Vector2f clampVec(sf::Vector2f a, float lengthLimit)
{
    if(length(a) > lengthLimit)
        return normalizeVec(a, lengthLimit);
    else
        return a;
}
float angleDifference(sf::Vector2f a, sf::Vector2f b)
{
    return -std::atan2(a.x * b.y - a.y * b.x, a.x * b.x + a.y * b.y);
}

float disVecCirc(sf::Vector2f a, sf::Vector2f P, float r)
{
    if(dot(a, P) <= 0.0f || std::abs(cross(a, P) / length(a)) >= r)
        return INFINITY;
    else
        return (dot(a, P) / length(a)) - std::sqrt(r * r - (float) std::pow(cross(a, P) / length(a), 2));
}
int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r)
{
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (abs(val) < CONST::FLOAT_EPS) return 0;  // collinear
    return (val > 0) ? 1 : 2;  // 1 -> clockwise, 2 -> counter-clockwise
}
bool helper_onSegment(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r)
{
    // Check if q is on the segment pr when the points are collinear
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return 1;
    return 0;
}
bool doIntersect(sf::Vector2f p1, sf::Vector2f q1, sf::Vector2f p2, sf::Vector2f q2)
{
    // Find the four orientations needed for general and special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return 1;

    // Special cases
    // p1, q1, p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && helper_onSegment(p1, p2, q1)) return true;
    // p1, q1, q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && helper_onSegment(p1, q2, q1)) return true;
    // p2, q2, p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && helper_onSegment(p2, p1, q2)) return true;
    // p2, q2, q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && helper_onSegment(p2, q1, q2)) return true;

    // Otherwise, they do not intersect
    return false;
}
sf::Vector2f rotate(sf::Vector2f a, float x)
{
    return sf::Vector2f(a.x * cos(x) - a.y * sin(x),
        a.x * sin(x) + a.y * cos(x));
}

sf::Vector2f randVecWithLength(float length)
{
    float arg = randBetween(-CONST::PI, CONST::PI);
    return rotate(sf::Vector2f(length, 0), arg);
}
sf::Vector2f randPointInScreen(float width, float height)
{
    float x = randBetween(0.0f, width);
    float y = randBetween(0.0f, height);
    return sf::Vector2f(x, y);
}

float mutate(float x)
{
    if(randBetween(0.0f, 1.0f) <= 0.001)
    {
        x *= randBetween(-2.0f, 2.0f);
    }
    if(randBetween(0.0f, 1.0f) <= 0.005)
    {
        x *= randBetween(0.8f, 1.2f);
    }
    if(randBetween(0.0f, 1.0f) <= 0.005)
    {
        x += randBetween(-0.01f, 0.01f);
    }
    return x;
}