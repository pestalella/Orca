#include "stdafx.h"
#include "scene.h"

Hit missShader(const Ray &r)
{
    return Hit(Vec3f(0), Vec3f(0), Vec3f(0.3, 0.3, 0.9), 1E5);
}

Hit Scene::intersectClosest(const Ray & r)
{
    Hit closestHit = missShader(r);
    for (auto curObj : objects) {
        Hit curHit = curObj->intersect(r);
        if (!curHit.miss) {
            if (closestHit.miss || curHit.t < closestHit.t) {
                closestHit = curHit;
            }
        }
    }
    return closestHit;
}

Hit Scene::intersectAny(const Ray & r)
{
    for (auto curObj : objects) {
        Hit curHit = curObj->intersect(r);
        if (!curHit.miss) {
            return curHit;
        }
    }
    return missShader(r);
}

Vec3f Scene::directIllumination(Vec3f const & p, Vec3f const & n)
{
    Vec3f directIllum(0);
    for (auto light : lights) {
        Vec3f lightAccum(0);
        int ns = 1;
        for (int sample = 0; sample < ns; ++sample) {
            Ray lightRay = light->generateLightRay(p);
            float cosNL = n.dotProduct(lightRay.dirNorm);
            if (cosNL > 1E-5) {
                Hit lightHit = intersectAny(lightRay);
                lightAccum = lightAccum + lightHit.color*cosNL;
            }
        }
        directIllum = directIllum + (lightAccum*(1.0 / ns));
    }
    return directIllum;
}

Scene::Scene()
{
    Vec3f noEmission(0);

    objects.push_back(new Ball(Vec3f(-1, -1, 2.5), 0.5, Vec3f(0, 0, 0), noEmission));
    objects.push_back(new Ball(Vec3f(1, -1, 2.5), 0.5, Vec3f(0, 0, 1), noEmission));
    objects.push_back(new Ball(Vec3f(-1, 1, 2.5), 0.5, Vec3f(0, 1, 0), noEmission));
    objects.push_back(new Ball(Vec3f(1, 1, 2.5), 0.5, Vec3f(0, 1, 1), noEmission));

    objects.push_back(new Ball(Vec3f(-1, -1, 0.5), 0.5, Vec3f(1, 0, 0), noEmission));
    objects.push_back(new Ball(Vec3f(1, -1, 0.5), 0.5, Vec3f(1, 0, 1), noEmission));
    objects.push_back(new Ball(Vec3f(-1, 1, 0.5), 0.5, Vec3f(1, 1, 0), noEmission));
    objects.push_back(new Ball(Vec3f(1, 1, 0.5), 0.5, Vec3f(1, 1, 1), noEmission));

    objects.push_back(new Plane(Vec3f(0, 0, -1), Vec3f(0, 0, 1), Vec3f(0.5, 0.5, 0.5)));

    lights.push_back(new Ball(Vec3f(10, -10, 10), 2.0, Vec3f(0, 0, 0), Vec3f(1, 1, 1)));
}
