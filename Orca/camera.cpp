#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <cmath>

#include "camera.h"

Camera::Camera(Vec3f org, Vec3f lookat, float fov, int width, int height) :
    origin(org), up(0, 0, 1), viewDir(lookat - org), fov(fov),
    widthPixels(width), heightPixels(height)
{
    viewDir.normalize();
    Vec3f right = viewDir.crossProduct(up);
    up = right.crossProduct(viewDir);

    cameraToWorld = Matrix44f(
        right.x, right.y, right.z, 0,
        up.x, up.y, up.z, 0,
        -viewDir.x, -viewDir.y, -viewDir.z, 0,
        origin.x, origin.y, origin.z, 1);

    std::cout << "0,0:" << createRay(0, 0).dirNorm << std::endl;
    std::cout << width << ",0:" << createRay(width, 0).dirNorm << std::endl;
    std::cout << width << "," << height << ":" << createRay(width, height).dirNorm << std::endl;
    std::cout << "0," << height << ":" << createRay(0, height).dirNorm << std::endl;
}

Ray Camera::createRay(int i, int j)
{
    float scale = tan(M_PI*(fov * 0.5) / 180.0);
    float imageAspectRatio = widthPixels / (float)heightPixels;
    float x = (2.0*(i + 0.5) / (float)widthPixels - 1)*imageAspectRatio*scale;
    float y = (1.0 - 2.0*(j + 0.5) / (float)heightPixels)*scale;
    Vec3f dir;
    cameraToWorld.multDirMatrix(Vec3f(x, y, -1), dir);
    dir.normalize();
    return Ray(origin, dir);
}
