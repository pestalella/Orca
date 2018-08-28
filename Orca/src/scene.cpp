#include "stdafx.h"
#include <cassert>

#include "BRDF.h"
#include "path.h"
#include "random.h"
#include "scene.h"

namespace Orca {

    //PathVertex missShader(Vec3f const &dir)
    //{
    //    static BRDF *skyBRDF = new DiffuseBRDF(1);
    //    static EDF *skyEDF = new IsotropicEDF(Vec3f(5300, 8000, 9200));

    //    return PathVertex(Vec3f(1E6f, 1E6f, 1E6f), -dir, dir, dir,
    //        Vec3f(1), skyBRDF, skyEDF);
    //}
    Vec3f missShader(Vec3f const &dir)
    {
//        return Vec3f(5300, 8000, 9200);
        return Vec3f(0, 0, 0);
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

    Scene::Scene(StatisticsCollector *sc) :
        sc(sc)
    {
        EDF *noEmission = new IsotropicEDF(0);
        BRDF *specular = new SpecularBRDF();
        cameraBRDF = new DiffuseBRDF(Vec3f(1));
        cameraEDF = new IsotropicEDF(Vec3f(0));  // The camera emits no light
        objects.push_back(new Ball(Vec3f(0, 0, 1.5f), 0.7f, specular, noEmission, sc));

        objects.push_back(new Ball(Vec3f(-1, -1, 2.5f), 0.5, new DiffuseBRDF(Vec3f(0.2f, 0.2f, 0.2f)), noEmission, sc));
        objects.push_back(new Ball(Vec3f( 1, -1, 2.5f), 0.5, new DiffuseBRDF(Vec3f(0, 0, 1)), noEmission, sc));
        objects.push_back(new Ball(Vec3f(-1,  1, 2.5f), 0.5, new DiffuseBRDF(Vec3f(1, 1, 0)), noEmission, sc));
        objects.push_back(new Ball(Vec3f( 1,  1, 2.5f), 0.5, new DiffuseBRDF(Vec3f(0, 1, 1)), noEmission, sc));

        objects.push_back(new Ball(Vec3f(-1, -1, 0.5f), 0.5, new DiffuseBRDF(Vec3f(1, 0, 0)), noEmission, sc));
        objects.push_back(new Ball(Vec3f( 1, -1, 0.5f), 0.5, new DiffuseBRDF(Vec3f(1, 0, 1)), noEmission, sc));
        objects.push_back(new Ball(Vec3f(-1,  1, 0.5f), 0.5, new DiffuseBRDF(Vec3f(1, 1, 0)), noEmission, sc));
        objects.push_back(new Ball(Vec3f( 1,  1, 0.5f), 0.5, new DiffuseBRDF(Vec3f(1, 1, 1)), noEmission, sc));
      
        Vec3f boxVerts[] = { {-1,-3,0}, {3,-3,0}, {3,3,0}, {-3,3,0},
                             {-3,-3,6}, {3,-3,6}, {3,3,6}, {-3,3,6} };

        // Cornell box walls
        // Floor
        objects.push_back(new Triangle(boxVerts[0], boxVerts[1], boxVerts[3], new DiffuseBRDF(Vec3f(0.5, 0.5, 0.5)), noEmission, sc));
        objects.push_back(new Triangle(boxVerts[1], boxVerts[2], boxVerts[3], new DiffuseBRDF(Vec3f(0.5, 0.5, 0.5)), noEmission, sc));
        // Right wall
        objects.push_back(new Triangle(boxVerts[1], boxVerts[5], boxVerts[6], new DiffuseBRDF(Vec3f(0.1, 1.0, 0.1)), noEmission, sc));
        objects.push_back(new Triangle(boxVerts[1], boxVerts[6], boxVerts[2], new DiffuseBRDF(Vec3f(0.1, 1.0, 0.1)), noEmission, sc));
        // Left wall
        objects.push_back(new Triangle(boxVerts[0], boxVerts[3], boxVerts[7], new DiffuseBRDF(Vec3f(1.0, 0.1, 0.1)), noEmission, sc));
        objects.push_back(new Triangle(boxVerts[0], boxVerts[7], boxVerts[4], new DiffuseBRDF(Vec3f(1.0, 0.1, 0.1)), noEmission, sc));
        // Back
        objects.push_back(new Triangle(boxVerts[2], boxVerts[6], boxVerts[7], new DiffuseBRDF(Vec3f(0.5, 0.5, 0.5)), noEmission, sc));
        objects.push_back(new Triangle(boxVerts[2], boxVerts[7], boxVerts[3], new DiffuseBRDF(Vec3f(0.5, 0.5, 0.5)), noEmission, sc));
        // Ceiling
        objects.push_back(new Triangle(boxVerts[5], boxVerts[7], boxVerts[6], new DiffuseBRDF(Vec3f(0.5, 0.5, 0.5)), noEmission, sc));
        objects.push_back(new Triangle(boxVerts[5], boxVerts[4], boxVerts[7], new DiffuseBRDF(Vec3f(0.5, 0.5, 0.5)), noEmission, sc));

        // Ceiling light
        Vec3f lv[] = { {1, -1, 5.95}, { 3, -1, 5.95 }, { 3, 1, 5.95 }, { 1, 1, 5.95 } };
        Triangle *ceil1 = new Triangle(lv[1], lv[3], lv[2], new DiffuseBRDF(Vec3f(1, 1, 1)), new IsotropicEDF(Vec3f(100000, 100000, 90000)), sc);
        Triangle *ceil2 = new Triangle(lv[1], lv[0], lv[3], new DiffuseBRDF(Vec3f(1, 1, 1)), new IsotropicEDF(Vec3f(100000, 100000, 90000)), sc);
        lights.push_back(ceil1);
        lights.push_back(ceil2);
        objects.push_back(ceil1);
        objects.push_back(ceil2);


        //// Floor
//        objects.push_back(new Plane(Vec3f(0, 0, 0), Vec3f(0, 0, 1), new DiffuseBRDF(Vec3f(0.7f, 0.7f, 0.7f)), noEmission, sc));

//        Ball *lightBulb = new Ball(Vec3f(5, -5, 5), 1.0, new DiffuseBRDF(Vec3f(1, 1, 1)), new IsotropicEDF(Vec3f(100000, 100000, 90000)), sc);
////        Ball *lightBulb = new Ball(Vec3f(5, -5, 5), 1.0, new DiffuseBRDF(Vec3f(1, 1, 1)), new IsotropicEDF(Vec3f(0, 0, 0)), sc);
//        lights.push_back(lightBulb);
//        objects.push_back(lightBulb);
    }

    Vec3f Scene::computeEnvLight(Vec3f const &pos, Vec3f const &normal)
    {
        Matrix44f orientation = orientationMatrix(normal);

        const int numEnvSamples = 10;
        Vec3f envLight = 0;
        for (int s = 0; s < numEnvSamples; ++s)
        {
            Vec3f rawDir = cosineSampleHemisphere();
            Vec3f rayDir;
            orientation.multDirMatrix(rawDir, rayDir);
            Hit hInfo = intersectAny(Ray(pos, rayDir));
            if (hInfo.miss) {
                envLight = (s*envLight + missShader(rayDir))*(1.0f/(s+1));
            }
        }
        return envLight;
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
//                randomPath.appendVertex(missShader(curNode.outDir));
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
        firstVertex.pos += firstVertex.normal*1E-4f;
        Path lightPath = buildPath(firstVertex, maxVertices);
        return lightPath;
    }

    Path Scene::buildCameraPath(Ray const &camRay, int maxVertices)
    {
        PathVertex cameraNode(camRay.origin, camRay.dirNorm, camRay.dirNorm, camRay.dirNorm, 1.0, cameraBRDF, cameraEDF);
        Path pathWithoutEnv = buildPath(cameraNode, maxVertices);
        return pathWithoutEnv;
        //PathVertex const &lastVert = pathWithoutEnv.vertices.back;
        //PathVertex envVertex(lastVert.outDir*1E5, -lastVert.outDir, lastVert.outDir, -lastVert.outDir)
        //pathWithoutEnv.appendVertex
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
            if (firstCamVert) {
                firstCamVert = false;
                if (viewPath.vertices.size() == 1)
                {
                    accumSamples = missShader(camVert.outDir);
                    numSamples++;
                }
                continue;
            }
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
                    Vec3f curSample = camVert.edf->emission(camVert.normal, camVert.outDir);

                    // Add light coming from the environment far away
                    curSample += computeEnvLight(camVert.pos + 1E-4f*camVert.normal, camVert.normal);

                    Hit hInfo = intersectClosest(Ray(camVert.pos + 1E-4f*camVert.normal, cam2light));
                    if (hInfo.t*hInfo.t > cam2lightNorm) {
                        Vec3f probCam = camVert.brdf->probability(camVert.normal, camVert.inDir, cam2light);
                        Vec3f probLight = lightVert.brdf->probability(lightVert.normal, lightVert.inDir, -cam2light);
                        Vec3f joiningPathProb = probLight*probCam;
                        Vec3f lightEmitProb = accumLightProbability * lightVert.edf->emission(lightVert.normal, lightVert.outDir);
                        curSample += joiningPathProb*lightEmitProb;
                    }
                    accumSamples += accumCamProbability*curSample;
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