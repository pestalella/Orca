#pragma once

#include "algebra.h"

namespace Orca
{
    class IntersectableObject;
    class BRDF;
    class EDF;

    class Hit
    {
    public:
        Vec3f pos;
        Vec3f normal;
        float t;  // hit distance along the ray
        bool miss;
        const IntersectableObject *object;
        const BRDF *brdf;
        const EDF *edf;

        Hit() :
            pos(0), normal(0), t(0), miss(true), object(0), brdf(0), edf(0)
        {}

        Hit(Vec3f const &pos, Vec3f const &normal, float t, 
            const IntersectableObject *obj, const BRDF *brdf, const EDF *edf) :
            pos(pos), normal(normal), t(t), miss(false), object(obj), brdf(brdf), edf(edf)
        {}
    };
}