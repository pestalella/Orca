#pragma once
#include "algebra.h"
#include "intersection.h"
#include "path.h"
#include "ray.h"

#include <vector>

namespace Orca
{
    class BRDF;

    class IntersectableObject
    {
    public:
        virtual Hit intersect(const Ray &r) const = 0;
        virtual Ray generateLightRay(const Vec3f &org) const { return Ray(Vec3f(0), Vec3f(0)); };
        virtual PathNode pointOnSurface() const = 0;
        virtual Vec3f emission() const { return Vec3f(0); }
        virtual const BRDF *getBRDF() const { return 0;  }
    };

    class Ball : public IntersectableObject
    {
    private:
        Vec3f center;
        float radius;
        float radius2;
        Vec3f color;
        Vec3f emitColor;
        const BRDF *brdf;

    public:
        Ball(Vec3f const &org, float radius, Vec3f const &color, const BRDF *brdf, Vec3f const &emitColor) :
            center(org), radius(radius), radius2(radius*radius),
            color(color), brdf(brdf), emitColor(emitColor)
        {}

        Hit intersect(const Ray &r) const override;
        Ray generateLightRay(const Vec3f &org) const override;
        PathNode pointOnSurface() const override;
        Vec3f emission() const override
        {
            return emitColor;
        }
        const BRDF *getBRDF() const override { return brdf; }

    };

    class Plane : public IntersectableObject
    {
    private:
        Vec3f center, normal;
        Vec3f color;
        const BRDF *brdf;

    public:
        Plane(Vec3f const &center, Vec3f const &normal, Vec3f const &color, const BRDF *brdf) :
            center(center), normal(normal), color(color), brdf(brdf)
        {}

        // A plane is not going to be used as an infinite-area light
        PathNode pointOnSurface() const override
        {
            return PathNode(Vec3f(0), Vec3f(0), Vec3f(0), 0, 0);
        }

        Hit intersect(const Ray &r) const override;
        const BRDF *getBRDF() const override { return brdf; }

    };
}