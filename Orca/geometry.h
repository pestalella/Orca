#pragma once
#include "algebra.h"
#include "intersection.h"
#include "ray.h"

#include <vector>

class IntersectableObject
{
public:
    virtual Hit intersect(const Ray &r) = 0;
    virtual Ray generateLightRay(const Vec3f &org)
    {
        return Ray(Vec3f(0), Vec3f(0));
    }
};

class Ball : public IntersectableObject
{
private:
    Vec3f origin;
    float radius;
    float radius2;
    Vec3f color;
    Vec3f emission;

public:
    Ball(Vec3f const &org, float radius, Vec3f const &color, Vec3f const &emission) :
        origin(org), radius(radius), radius2(radius*radius),
        color(color), emission(emission)
    {}

    Hit intersect(const Ray &r);
    Ray generateLightRay(const Vec3f &org);

};

class Plane : public IntersectableObject
{
private:
    Vec3f center, normal;
    Vec3f color;

public:
    Plane(Vec3f const &center, Vec3f const &normal, Vec3f const &color) :
        center(center), normal(normal), color(color)
    {}

    Hit intersect(const Ray &r);
};

