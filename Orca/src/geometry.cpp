#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "geometry.h"
#include "random.h"
#include "scene.h"

namespace Orca {

    // assumes a = 1
    bool solveQuadratic(float b, float c, float &x0, float &x1)
    {
        float discr = b * b - 4.0f * c;
        if (discr < 0.0) return false;
        else if (std::abs(discr) < 1E-6f) x0 = x1 = -0.5f * b;
        else {
            float q = (b > 0.0f) ?
                -0.5f * (b + sqrt(discr)) :
                -0.5f * (b - sqrt(discr));
            x0 = q;
            x1 = c / q;
        }
        if (x0 > x1) std::swap(x0, x1);

        return true;
    }

    Hit Plane::intersect(const Ray & r) const
    {
        stats->accumEvent("Plane::intersect");

        float denom = normal.dotProduct(r.dirNorm);
        if (std::abs(denom) > 1E-6f) // your favorite epsilon
        {
            float t = (center - r.origin).dotProduct(normal) / denom;
            if (t > 1E-6f) {
                Vec3f hitPoint = r.origin + t * r.dirNorm;
                return Hit(hitPoint, normal, t, this, brdf, edf);
            }
        }
        return Hit();
    }

    Hit Ball::intersect(const Ray & r) const
    {
        stats->accumEvent("Ball::intersect");

        float t0, t1; // solutions for t if the ray intersects 
        // analytic solution
        Vec3f L = r.origin - center;
        float b = 2.0f * r.dirNorm.dotProduct(L);
        float c = L.dotProduct(L) - radius2;
        if (!solveQuadratic(b, c, t0, t1)) return Hit();

        if (t0 < 0.0f) {
            t0 = t1; // if t0 is negative, let's use t1 instead 
            if (t0 < 0.0f) return Hit(); // both t0 and t1 are negative 
        }

        Vec3f hitPoint = r.origin + r.dirNorm*t0;
        Vec3f hitNormal = hitPoint - center;
        hitNormal.normalize();

        return Hit(hitPoint, hitNormal, t0, this, brdf, edf);
    }

    Ray Ball::generateLightRay(const Vec3f &org) const
    {
        stats->accumEvent("Plane::generateLightRay");

        float rho = sqrt(Random::uniform01());
        rho *= radius;
        float theta = Random::uniform01()*(float)M_2_PI;
        float x = rho*cos(theta);
        float y = rho*sin(theta);
        Vec3f o2c = center - org;
        o2c.normalize();
        Vec3f up(0, 0, 1);
        Vec3f discUp = o2c.crossProduct(Vec3f(0, 0, 1));
        if (discUp.norm() < 1E-2)
            discUp = o2c.crossProduct(Vec3f(0, 1, 0));
        if (discUp.norm() < 1E-2)
            discUp = o2c.crossProduct(Vec3f(1, 0, 0));
        discUp.normalize();
        Vec3f discRight = discUp.crossProduct(o2c);
        Vec3f rayDir = center + discRight*x + discUp*y - org;
        rayDir.normalize();
        return Ray(org, rayDir);
    }

    PathVertex Ball::pointOnSurface() const
    {
        stats->accumEvent("Ball::pointOnSurface");

        float x1, x2;
        do {
            x1 = Random::uniform(-1, 1);
            x2 = Random::uniform(-1, 1);
        } while ((x1*x1 + x2*x2) >= 1.0f);
        float x1sq = x1*x1;
        float x2sq = x2*x2;
        float sqrtDisc = sqrt(1 - x1sq - x2sq);

        Vec3f pos(2.0f*x1*sqrtDisc,
            2.0f*x2*sqrtDisc,
            1 - 2*(x1sq+x2sq));
        Vec3f normal(pos);
        normal.normalize();

        // TODO: Review the outDir (make it cos-weighted diffuse?)
        return PathVertex(center+pos*radius, normal, -normal, normal, 1.0f, brdf, edf);
    }

    Vec3f cosineSampleHemisphere()
    {
        float u1 = Random::uniform01();
        float u2 = Random::uniform01();
        float r = sqrt(u1);
        float theta = 2 * M_PI * u2;

        float x = r * cos(theta);
        float y = r * sin(theta);
        
        return Vec3f(x, y, sqrt(std::max(0.0f, 1 - u1)));
    }
}