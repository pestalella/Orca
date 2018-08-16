#pragma once

#include "algebra.h"

class Hit
{
public:
    Vec3f pos;
    Vec3f normal;
    Vec3f color;
    float t;  // hit distance along the ray
    bool miss;

    Hit(Vec3f pos, Vec3f normal, Vec3f color, float t) :
        pos(pos), normal(normal), color(color), t(t), miss(false)
    {}
};


