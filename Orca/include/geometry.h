#pragma once
#include "algebra.h"
#include "intersection.h"
#include "path.h"
#include "ray.h"

#include <vector>

namespace Orca
{
    class BRDF;
    class EDF;

    class IntersectableObject
    {
    public:
        virtual Hit intersect(const Ray &r) const = 0;
        virtual Ray generateLightRay(const Vec3f &org) const { return Ray(Vec3f(0), Vec3f(0)); };
        virtual PathVertex pointOnSurface() const = 0;
        virtual const BRDF *getBRDF() const { return 0; }
        virtual const EDF *getEDF() const { return 0; }
    };

    class Ball : public IntersectableObject
    {
    private:
        Vec3f center;
        float radius;
        float radius2;
        const BRDF *brdf;
        const EDF *edf;

    public:
        Ball(Vec3f const &org, float radius, const BRDF *brdf, const EDF *edf) :
            center(org), radius(radius), radius2(radius*radius),
            brdf(brdf), edf(edf)
        {}

        Hit intersect(const Ray &r) const override;
        Ray generateLightRay(const Vec3f &org) const override;
        PathVertex pointOnSurface() const override;
        const EDF *getEDF() const override { return edf; }
        const BRDF *getBRDF() const override { return brdf; }

    };

    class Plane : public IntersectableObject
    {
    private:
        Vec3f center, normal;
        const BRDF *brdf;
        const EDF *edf;

    public:
        Plane(Vec3f const &center, Vec3f const &normal, const BRDF *brdf, const EDF *edf) :
            center(center), normal(normal), brdf(brdf), edf(edf)
        {}

        // A plane is not going to be used as an infinite-area light
        PathVertex pointOnSurface() const override
        {
            return PathVertex(Vec3f(0), Vec3f(0), Vec3f(0), Vec3f(0), 0, 0, 0);
        }

        Hit intersect(const Ray &r) const override;
        const BRDF *getBRDF() const override { return brdf; }
        const EDF *getEDF() const override { return edf; }
    };
}