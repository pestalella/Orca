#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <cmath>

#include "geometry.h"
#include "scene.h"

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (std::abs(discr) < 1E-6) x0 = x1 = -0.5 * b / a;
    else {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);

    return true;
}

Hit Plane::intersect(const Ray & r)
{
    float denom = normal.dotProduct(r.dirNorm);
    if (std::abs(denom) > 0.0001f) // your favorite epsilon
    {
        float t = (center - r.origin).dotProduct(normal) / denom;
        if (t > 0.0001f) {
            Vec3f hitPoint = r.origin + t * r.dirNorm;
            return Hit(hitPoint, normal, color, t);
        }
    }
    return missShader(r);
}

Hit Ball::intersect(const Ray & r)
{
    float t0, t1; // solutions for t if the ray intersects 
    // analytic solution
    Vec3f L = r.origin - origin;
    float a = 1.0;
    float b = 2 * r.dirNorm.dotProduct(L);
    float c = L.dotProduct(L) - radius2;
    if (!solveQuadratic(a, b, c, t0, t1)) return missShader(r);
    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead 
        if (t0 < 0) return missShader(r); // both t0 and t1 are negative 
    }

    Vec3f hitPoint = r.origin + r.dirNorm*t0;
    Vec3f hitNormal = hitPoint - origin;
    hitNormal.normalize();
    return Hit(hitPoint, hitNormal, color + emission, t0);
}

Ray Ball::generateLightRay(const Vec3f &org)
{
    float rho = sqrt(rand() / (double)RAND_MAX);
    rho *= radius;
    float theta = rand() / (double)RAND_MAX * M_2_PI;
    float x = rho * cos(theta);
    float y = rho * sin(theta);
    Vec3f o2c = origin - org;
    o2c.normalize();
    Vec3f up(0, 0, 1);
    Vec3f discUp = o2c.crossProduct(Vec3f(0, 0, 1));
    if (discUp.norm() < 1E-5)
        Vec3f perp1 = o2c.crossProduct(Vec3f(0, 1, 0));
    discUp.normalize();
    Vec3f discRight = discUp.crossProduct(o2c);
    discRight.normalize();
    Vec3f rayDir = (origin + discRight * x + discUp * y - org);
    rayDir.normalize();
    return Ray(org, rayDir);
}
