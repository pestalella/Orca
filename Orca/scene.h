#pragma once

#include "geometry.h"
#include "ray.h"

#include <vector>

class Scene
{
private:
    std::vector<IntersectableObject *> objects;
    std::vector<IntersectableObject *> lights;

public:
    Scene();
    Hit intersectClosest(const Ray &r);
    Hit intersectAny(const Ray &r);

    Vec3f directIllumination(Vec3f const &p, Vec3f const & n);
};

Hit missShader(const Ray &r);
