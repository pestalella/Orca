#include "stdafx.h"

#include "algebra.h"

using Orca::Vec3f;

Orca::Matrix44f Orca::orientationMatrix(Vec3f const & normal)
{
    Vec3f up = normal.crossProduct(Vec3f(0, 0, 1));
    if (up.norm() < 0.1f)
        up = normal.crossProduct(Vec3f(0, 1, 0));
    if (up.norm() < 0.1f)
        up = normal.crossProduct(Vec3f(1, 0, 0));
    up.normalize();
    Vec3f right = up.crossProduct(normal);
    right.normalize();

    return Matrix44f( right[0],  right[1],  right[2], 0, 
                         up[0],     up[1],     up[2], 0,
                     normal[0], normal[1], normal[2], 0,
                             0,         0,         0, 1);
}
