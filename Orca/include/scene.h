#pragma once

#include "geometry.h"
#include "ray.h"

#include <vector>

namespace Orca
{
    class Scene
    {
    private:
        std::vector<IntersectableObject *> objects;
        std::vector<IntersectableObject *> lights;

    private:
        Path buildPath(PathNode const &startNode, int maxNodes);

    public:
        Scene();
        Hit intersectClosest(const Ray &r);
        Hit intersectAny(const Ray &r);

        Vec3f directIllumination(Vec3f const &p, Vec3f const & n);
        Orca::Path buildLightPath(int maxNodes);
        Orca::Path buildCameraPath(Ray const &camRay, int maxNodes);


        Vec3f traceRayRecursive(Ray const &r, int level);

    };

    Vec3f missShader(const Ray &r);
}