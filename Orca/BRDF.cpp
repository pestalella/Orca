#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "BRDF.h"
#include "random.h"

using Orca::Vec3f;

Orca::BRDF::BRDF()
{
}


Orca::BRDF::~BRDF()
{
}

Vec3f Orca::DiffuseBRDF::generateOutSample(Vec3f const & normal, Vec3f const & /*unused: inDir */) const
{
    float u = Random::uniform01();
    float v = Random::uniform01();

    float phi = (float)(2.0 * M_PI*u);
    float cosTheta = sqrt(1.0f - v);
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
    Vec3f unoriented(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
    Vec3f outSample;
    orientationMatrix(normal).multDirMatrix(unoriented, outSample);
    return outSample;
}

Vec3f Orca::SpecularBRDF::generateOutSample(Vec3f const & normal, Vec3f const & inDir) const
{
    return 2.0*normal + inDir;
}
