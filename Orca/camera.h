#pragma once

#include "algebra.h"
#include "ray.h"

class Camera
{
private:
    Vec3f origin;
    Vec3f up;
    Vec3f viewDir;
    float fov;
    int widthPixels, heightPixels;
    Matrix44f cameraToWorld;

public:
    Camera(Vec3f org, Vec3f lookat, float fov, int width, int height);

    Ray createRay(int i, int j);

};

