// Orca.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <fstream>

#include "algebra.h"
#include "camera.h"
#include "geometry.h"
#include "intersection.h"
#include "ray.h"
#include "scene.h"


int main()
{
    const int imgWidth = 500, imgHeight = 500;
    std::vector<Vec3f> imageBuf(imgWidth*imgHeight);

    Scene scn;
    Camera cam(Vec3f(-4, -15, 4), Vec3f(0, 0, 2), 30, imgWidth, imgHeight);

    std::cout << "Raytracing..." << std::endl;
    for (int i = 0; i < imgHeight; ++i) {
        for (int j = 0; j < imgWidth; ++j) {
            Hit hInfo = scn.intersectClosest(cam.createRay(j, i));
            if (hInfo.miss) {
                imageBuf[i*imgWidth + j] = Vec3f(1, 0, 0);
            } else {
                imageBuf[i*imgWidth + j] = hInfo.color * scn.directIllumination(hInfo.pos + (hInfo.normal*1E-4), hInfo.normal);
            }
        }
    }

    std::cout << "Writing image to disk" << std::endl;
    std::ofstream outFile("out.ppm", std::ios::out | std::ios::binary);
    outFile << "P6\n" << imgWidth << " " << imgHeight << "\n255\n";
    for (int i = 0; i < imgHeight; i++) {
        for (int j = 0; j < imgWidth; j++) {
            Vec3f col = imageBuf[i*imgWidth + j];
//            col = Vec3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            char ir = int(255.99*col[0]);
            char ig = int(255.99*col[1]);
            char ib = int(255.99*col[2]);
            outFile << ir << ig << ib;
        }
    }
    return 0;
}

