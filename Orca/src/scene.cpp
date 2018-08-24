#include "stdafx.h"
#include <cassert>

#include "BRDF.h"
#include "path.h"
#include "random.h"
#include "scene.h"

namespace Orca {

    PathVertex missShader(Vec3f const &dir)
    {
        static BRDF *skyBRDF = new DiffuseBRDF(1);
        static EDF *skyEDF = new IsotropicEDF(Vec3f(5300, 8000, 9200));

        return PathVertex(Vec3f(1E6f, 1E6f, 1E6f), -dir, dir, dir, 
            Vec3f(1), skyBRDF, skyEDF);
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

    Scene::Scene()
    {
        EDF *noEmission = new IsotropicEDF(0);
        BRDF *specular = new SpecularBRDF();
        cameraBRDF = new DiffuseBRDF(Vec3f(1));
        cameraEDF = new IsotropicEDF(Vec3f(0));  // The camera emits no light
        objects.push_back(new Ball(Vec3f(0, 0, 1.5f), 0.7f, specular, noEmission));

        objects.push_back(new Ball(Vec3f(-1, -1, 2.5f), 0.5, new DiffuseBRDF(Vec3f(0.2f, 0.2f, 0.2f)), noEmission));
        objects.push_back(new Ball(Vec3f( 1, -1, 2.5f), 0.5, new DiffuseBRDF(Vec3f(0, 0, 1)), noEmission));
        objects.push_back(new Ball(Vec3f(-1,  1, 2.5f), 0.5, new DiffuseBRDF(Vec3f(1, 1, 0)), noEmission));
        objects.push_back(new Ball(Vec3f( 1,  1, 2.5f), 0.5, new DiffuseBRDF(Vec3f(0, 1, 1)), noEmission));

        objects.push_back(new Ball(Vec3f(-1, -1, 0.5f), 0.5, new DiffuseBRDF(Vec3f(1, 0, 0)), noEmission));
        objects.push_back(new Ball(Vec3f( 1, -1, 0.5f), 0.5, new DiffuseBRDF(Vec3f(1, 0, 1)), noEmission));
        objects.push_back(new Ball(Vec3f(-1,  1, 0.5f), 0.5, new DiffuseBRDF(Vec3f(1, 1, 0)), noEmission));
        objects.push_back(new Ball(Vec3f( 1,  1, 0.5f), 0.5, new DiffuseBRDF(Vec3f(1, 1, 1)), noEmission));

        // Floor
        objects.push_back(new Plane(Vec3f(0, 0, 0), Vec3f(0, 0, 1), new DiffuseBRDF(Vec3f(0.7f, 0.7f, 0.7f)), noEmission));

        Ball *lightBulb = new Ball(Vec3f(5, -5, 5), 1.0, new DiffuseBRDF(Vec3f(1, 1, 1)), new IsotropicEDF(Vec3f(100000, 100000, 90000)));
        lights.push_back(lightBulb);
        objects.push_back(lightBulb);
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
                randomPath.appendVertex(missShader(curNode.outDir));
                break;
            }
            BRDFSample outSample = hInfo.brdf->generateOutSample(hInfo.normal, curNode.outDir);
            curNode = PathVertex(hInfo.pos, hInfo.normal, curNode.outDir, outSample.dir, outSample.probability, hInfo.brdf, hInfo.edf);
            randomPath.appendVertex(curNode);
            numNodes++;
        }
        return randomPath;
    }

    Path Scene::buildLightPath(int maxVertices)
    {
        int selectedLight = Random::uniformInt(0, (int)lights.size()-1);
        IntersectableObject *light = lights[selectedLight];
        auto firstVertex = light->pointOnSurface();
        firstVertex.pos += firstVertex.normal*1E-4;
        Path lightPath = buildPath(firstVertex, maxVertices);
        return lightPath;
    }

    Path Scene::buildCameraPath(Ray const &camRay, int maxVertices)
    {
        PathVertex cameraNode(camRay.origin, camRay.dirNorm, camRay.dirNorm, camRay.dirNorm, 1.0, cameraBRDF, cameraEDF);
        return buildPath(cameraNode, maxVertices);
    }

    Vec3f Scene::traceRayBidirectional(Ray const &r, int maxVertices)
    {
        Path viewPath = buildCameraPath(r, maxVertices);
        Path lightPath = buildLightPath(maxVertices);

        Vec3f accumCamProbability = 1.0;
        Vec3f accumSamples = 0;
        int numSamples = 0;
        bool firstCamVert = true;
        for (auto &camVert : viewPath.vertices) {
            Vec3f accumLightProbability = 1.0;
            accumCamProbability *= camVert.probability;
            bool firstLightVert = true;
            for (auto &lightVert: lightPath.vertices) {
                if (!(firstCamVert && firstLightVert)) {
                    accumLightProbability *= lightVert.probability;
                    Vec3f cam2light = (lightVert.pos - camVert.pos);
                    float cam2lightNorm = cam2light.norm();
                    cam2light *= 1.0f/sqrt(cam2lightNorm);

                    // Add local emission from the camera path vertex
                    Vec3f camEmitProb = accumCamProbability * camVert.edf->emission(camVert.normal, camVert.outDir);
                    accumSamples += camEmitProb;

                    Hit hInfo = intersectClosest(Ray(camVert.pos + 1E-4f*camVert.normal, cam2light));
                    if (hInfo.t*hInfo.t >= cam2lightNorm) {
                        Vec3f probCam = camVert.brdf->probability(camVert.normal, camVert.inDir, cam2light);
                        Vec3f probLight = lightVert.brdf->probability(lightVert.normal, lightVert.inDir, -cam2light);
                        Vec3f joiningPathProb = probLight*probCam;
                        Vec3f lightEmitProb = accumLightProbability * lightVert.edf->emission(lightVert.normal, lightVert.outDir);
                        accumSamples += joiningPathProb*lightEmitProb;
                    }
                    numSamples++;
                }
                firstLightVert = false;

            }
            firstCamVert = false;
        }
        if (numSamples == 0)
            return 0;
        else
            return accumSamples*(1.0f/numSamples);
    }
}