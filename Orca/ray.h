#pragma once
#include "algebra.h"

namespace Orca
{
    class Ray
    {
    public:
        Vec3f origin;
        Vec3f dirNorm;

        Ray(const Vec3f &org, const Vec3f &dir) :
            origin(org), dirNorm(dir)
        {}
        friend std::ostream& operator << (std::ostream &s, const Ray &r)
        {
            return s << "(org:" << r.origin << ", dir:" << r.dirNorm << ")";
        }
    };
}