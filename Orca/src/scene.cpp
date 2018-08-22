#include "stdafx.h"

#include "BRDF.h"
#include "path.h"
#include "random.h"
#include "scene.h"

using Orca::Hit;
using Orca::Ray;
using Orca::Vec3f;

Vec3f Orca::missShader(const Ray &r)
{
    return Vec3f(700, 700, 7000);
}

Hit Orca::Scene::intersectClosest(const Ray & r)
{
    Hit closestHit;
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

Hit Orca::Scene::intersectAny(const Ray & r)
{
    for (auto curObj : objects) {
        Hit curHit = curObj->intersect(r);
        if (!curHit.miss) {
            return curHit;
        }
    }
    return Hit();
}

Vec3f Orca::Scene::directIllumination(Vec3f const & p, Vec3f const & n)
{
    Vec3f directIllum(0);
    Vec3f displacedHit = p + 1E-3f*n;
    for (auto light : lights) {
        Vec3f lightAccum(0);
        int ns = 4;
        for (int sample = 0; sample < ns; ++sample) {
            Ray lightRay = light->generateLightRay(displacedHit);
            float cosNL = n.dotProduct(lightRay.dirNorm);
            if (cosNL > 1E-5) {
                Hit lightHit = intersectAny(lightRay);
                if (lightHit.miss) {
                    lightAccum += light->emission()*cosNL;
                }
            }   
        }
        directIllum += (lightAccum*(1.0f / ns));
    }
    return directIllum;
}

Orca::Scene::Scene()
{
    Vec3f noEmission(0);
    BRDF *diffuse = new DiffuseBRDF();
    BRDF *specular = new SpecularBRDF();
    objects.push_back(new Ball(Vec3f( 0,  0, 1.5f), 0.7f, Vec3f(1, 1, 1), specular, noEmission));

    objects.push_back(new Ball(Vec3f(-1, -1, 2.5f), 0.5, Vec3f(0.2f, 0.2f, 0.2f), diffuse, noEmission));
    objects.push_back(new Ball(Vec3f( 1, -1, 2.5f), 0.5, Vec3f(0, 0, 1), diffuse, noEmission));
    objects.push_back(new Ball(Vec3f(-1,  1, 2.5f), 0.5, Vec3f(0, 1, 0), diffuse, noEmission));
    objects.push_back(new Ball(Vec3f( 1,  1, 2.5f), 0.5, Vec3f(0, 1, 1), diffuse, noEmission));

    objects.push_back(new Ball(Vec3f(-1, -1, 0.5f), 0.5, Vec3f(1, 0, 0), diffuse, noEmission));
    objects.push_back(new Ball(Vec3f( 1, -1, 0.5f), 0.5, Vec3f(1, 0, 1), diffuse, noEmission));
    objects.push_back(new Ball(Vec3f(-1,  1, 0.5f), 0.5, Vec3f(1, 1, 0), diffuse, noEmission));
    objects.push_back(new Ball(Vec3f( 1,  1, 0.5f), 0.5, Vec3f(1, 1, 1), diffuse, noEmission));

    // Floor
    objects.push_back(new Plane(Vec3f(0, 0, 0), Vec3f(0, 0, 1), Vec3f(0.7f, 0.7f, 0.7f), diffuse));

    lights.push_back(new Ball(Vec3f(100, -100, 100), 2.0, Vec3f(0, 0, 0), diffuse, Vec3f(10000, 10000, 9000)));
}

Orca::Path Orca::Scene::buildPath(PathNode const &startNode, int maxNodes)
{
    Path randomPath;

    randomPath.appendNode(startNode);
    PathNode curNode = startNode;
    int numNodes = 1;
    while (numNodes < maxNodes) {
        Hit hInfo = intersectClosest(Ray(curNode.pos + 1E-4f*curNode.normal, curNode.outDir));
        if (hInfo.miss) {
            // Missed the scene. Stop trying to find new nodes.
            break;
        }
        Orca::BRDFSample outSample = hInfo.brdf->generateOutSample(hInfo.normal, curNode.outDir);
        curNode = PathNode(hInfo.pos, hInfo.normal, outSample.dir, outSample.probability, hInfo.brdf);
        randomPath.appendNode(curNode);
        numNodes++;
    }
    return randomPath;

}

Orca::Path Orca::Scene::buildLightPath(int maxNodes)
{
    int selectedLight = (int)(Orca::Random::uniform01()*lights.size());
    IntersectableObject *light = lights[selectedLight];
    Path lightPath = buildPath(light->pointOnSurface(), maxNodes);
    return lightPath;
}

Orca::Path Orca::Scene::buildCameraPath(Ray const &camRay, int maxNodes)
{
    PathNode cameraNode(camRay.origin, camRay.dirNorm, camRay.dirNorm, 1.0, 0);
    return buildPath(cameraNode, maxNodes);
}

Vec3f Orca::Scene::traceRayRecursive(Ray const &r, int level)
{
    Vec3f resultColor(0);
    Hit hInfo = intersectClosest(r);
    if (hInfo.miss) {
        resultColor = missShader(r);
    }
    else {
        resultColor = hInfo.color*directIllumination(hInfo.pos + (hInfo.normal*1E-4f), hInfo.normal);
        Vec3f incomingLight(0);
        if (level > 0) {
            const BRDF *hitbrdf = hInfo.brdf;
            int ns = 1;
            for (int i = 0; i < ns; ++i) {
                BRDFSample outSample = hitbrdf->generateOutSample(hInfo.normal, r.dirNorm);
                Ray recRay(hInfo.pos + 1E-3f*hInfo.normal, outSample.dir);
                incomingLight += hInfo.color*traceRayRecursive(recRay, level - 1);
            }
            incomingLight *= 1.0f / ns;
        }
        resultColor += incomingLight;
    }
    return resultColor;
}
