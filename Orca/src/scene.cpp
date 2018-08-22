#include "stdafx.h"

#include "BRDF.h"
#include "path.h"
#include "random.h"
#include "scene.h"

namespace Orca {

    Vec3f missShader(const Ray &r)
    {
        return Vec3f(700, 700, 7000);
    }

    Hit Scene::intersectClosest(const Ray & r)
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

    Hit Scene::intersectAny(const Ray & r)
    {
        for (auto curObj : objects) {
            Hit curHit = curObj->intersect(r);
            if (!curHit.miss) {
                return curHit;
            }
        }
        return Hit();
    }

    Vec3f Scene::directIllumination(Vec3f const & p, Vec3f const & n)
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

    Scene::Scene()
    {
        Vec3f noEmission(0);
        BRDF *diffuse = new DiffuseBRDF();
        BRDF *specular = new SpecularBRDF();
        cameraDF = new IsotropicDF();
        objects.push_back(new Ball(Vec3f(0, 0, 1.5f), 0.7f, Vec3f(1, 1, 1), specular, noEmission));

        objects.push_back(new Ball(Vec3f(-1, -1, 2.5f), 0.5, Vec3f(0.2f, 0.2f, 0.2f), diffuse, noEmission));
        objects.push_back(new Ball(Vec3f(1, -1, 2.5f), 0.5, Vec3f(0, 0, 1), diffuse, noEmission));
        objects.push_back(new Ball(Vec3f(-1, 1, 2.5f), 0.5, Vec3f(0, 1, 0), diffuse, noEmission));
        objects.push_back(new Ball(Vec3f(1, 1, 2.5f), 0.5, Vec3f(0, 1, 1), diffuse, noEmission));

        objects.push_back(new Ball(Vec3f(-1, -1, 0.5f), 0.5, Vec3f(1, 0, 0), diffuse, noEmission));
        objects.push_back(new Ball(Vec3f(1, -1, 0.5f), 0.5, Vec3f(1, 0, 1), diffuse, noEmission));
        objects.push_back(new Ball(Vec3f(-1, 1, 0.5f), 0.5, Vec3f(1, 1, 0), diffuse, noEmission));
        objects.push_back(new Ball(Vec3f(1, 1, 0.5f), 0.5, Vec3f(1, 1, 1), diffuse, noEmission));

        // Floor
        objects.push_back(new Plane(Vec3f(0, 0, 0), Vec3f(0, 0, 1), Vec3f(0.7f, 0.7f, 0.7f), diffuse));

        lights.push_back(new Ball(Vec3f(100, -100, 100), 2.0, Vec3f(0, 0, 0), diffuse, Vec3f(10000, 10000, 9000)));
    }

    Path Scene::buildPath(PathVertex const &startNode, int maxVertices)
    {
        Path randomPath;

        randomPath.appendVertex(startNode);
        PathVertex curNode = startNode;
        int numNodes = 1;
        while (numNodes < maxVertices) {
            Hit hInfo = intersectClosest(Ray(curNode.pos + 1E-4f*curNode.normal, curNode.outDir));
            if (hInfo.miss) {
                // Missed the scene. Stop trying to find new nodes.
                break;
            }
            BRDFSample outSample = hInfo.brdf->generateOutSample(hInfo.normal, curNode.outDir);
            curNode = PathVertex(hInfo.pos, hInfo.normal, curNode.outDir, outSample.dir, outSample.probability, hInfo.brdf);
            randomPath.appendVertex(curNode);
            numNodes++;
        }
        return randomPath;
    }

    Path Scene::buildLightPath(int maxVertices)
    {
        int selectedLight = Random::uniformInt(0, lights.size()-1);
        IntersectableObject *light = lights[selectedLight];
        Path lightPath = buildPath(light->pointOnSurface(), maxVertices);
        lightPath.originalLightIntensity = light->emission();
        return lightPath;
    }

    Path Scene::buildCameraPath(Ray const &camRay, int maxVertices)
    {
        PathVertex cameraNode(camRay.origin, camRay.dirNorm, camRay.dirNorm, camRay.dirNorm, 1.0, cameraDF);
        return buildPath(cameraNode, maxVertices);
    }

    Vec3f Scene::traceRayBidirectional(Ray const &r, int maxVertices)
    {
        Path viewPath = buildCameraPath(r, maxVertices);
        Path lightPath = buildLightPath(maxVertices);

        float accumCamProbability = 1.0;
        Vec3f accumSamples = 0;
        int numSamples = 0;

        for (auto &camVert : viewPath.vertices) {
            float accumLightProbability = 1.0;
            accumCamProbability *= camVert.probability;
            for (auto &lightVert: lightPath.vertices) {
                accumLightProbability *= lightVert.probability;
                Vec3f cam2light = (lightVert.pos - camVert.pos).normalized();
                float probCam = camVert.brdf->probability(camVert.normal, camVert.inDir, cam2light);
                float probLight = lightVert.brdf->probability(lightVert.normal, lightVert.inDir, -cam2light);
                accumSamples += accumLightProbability * accumCamProbability * probCam * probLight * lightPath.originalLightIntensity;
                numSamples++;
            }
        }
        return accumSamples*(1.0f/numSamples);
    }


    Vec3f Scene::traceRayRecursive(Ray const &r, int level)
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

}