#pragma once
#include "algebra.h"
#include "intersection.h"
#include "path.h"
#include "ray.h"
#include "statistics_collector.h"

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
        StatisticsCollector *stats;

    public:
        Ball(Vec3f const &org, float radius, const BRDF *brdf, const EDF *edf, StatisticsCollector *stats) :
            center(org), radius(radius), radius2(radius*radius),
            brdf(brdf), edf(edf), stats(stats)
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
        StatisticsCollector *stats;

    public:
        Plane(Vec3f const &center, Vec3f const &normal, const BRDF *brdf, const EDF *edf, StatisticsCollector *stats) :
            center(center), normal(normal), brdf(brdf), edf(edf), stats(stats)
        {}

        // A plane is not going to be used as an infinite-area light
        PathVertex pointOnSurface() const override
        {
            stats->accumEvent("Plane::PointOnSurface");
            return PathVertex(Vec3f(0), Vec3f(0), Vec3f(0), Vec3f(0), 0, 0, 0);
        }

        Hit intersect(const Ray &r) const override;
        const BRDF *getBRDF() const override { return brdf; }
        const EDF *getEDF() const override { return edf; }
    };

    class Triangle : public IntersectableObject
    {
    private: 
        Vec3f v0, v1, v2;
        Vec3f edge1, edge2;
        Vec3f normal;
        const BRDF *brdf;
        const EDF *edf;
        StatisticsCollector *stats;

    public:
        Triangle(Vec3f const &v0, Vec3f const &v1, Vec3f const &v2, 
                 const BRDF *brdf, const EDF *edf, StatisticsCollector *stats);

        PathVertex pointOnSurface() const override;
        Hit intersect(const Ray &r) const override;
        const BRDF *getBRDF() const override { return brdf; }
        const EDF *getEDF() const override { return edf; }
    };

    Vec3f cosineSampleHemisphere();
}

