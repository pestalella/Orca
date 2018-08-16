#pragma once
#include "algebra.h"

class Ray
{
public:
    Vec3f origin;
    Vec3f dirNorm;

    Ray(Vec3f org, Vec3f dir) :
        origin(org), dirNorm(dir)
    {}
};
