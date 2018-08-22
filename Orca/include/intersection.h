#pragma once

#include "algebra.h"

namespace Orca
{
    class IntersectableObject;
    class BRDF;

    class Hit
    {
    public:
        Vec3f pos;
        Vec3f normal;
        Vec3f color;
        float t;  // hit distance along the ray
        bool miss;
        const IntersectableObject *object;
        const BRDF *brdf;

        Hit() :
            pos(0), normal(0), color(0), t(0), miss(true), object(0), brdf(0)
        {}

        Hit(Vec3f const &pos, Vec3f const &normal, Vec3f const &color, float t, const IntersectableObject *obj, const BRDF *brdf) :
            pos(pos), normal(normal), color(color), t(t), miss(false), object(obj), brdf(brdf)
        {}
    };
}