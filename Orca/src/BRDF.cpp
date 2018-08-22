#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "BRDF.h"
#include "random.h"

namespace Orca {

    BRDF::BRDF()
    {
    }


    BRDF::~BRDF()
    {
    }

    float DiffuseBRDF::probability(Vec3f const & normal, Vec3f const & inDir, Vec3f const & outDir) const
    {
        // isotropic, therefore only depends on the cosine of the input dir wrt the normal
        return normal.dotProduct(inDir);
    }

    BRDFSample DiffuseBRDF::generateOutSample(Vec3f const & normal, Vec3f const &inDir) const
    {
        float u = Random::uniform01();
        float v = Random::uniform01();

        float phi = (float)(2.0 * M_PI*u);
        float cosTheta = sqrt(1.0f - v);
        float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
        Vec3f unoriented(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
        Vec3f outDir;
        orientationMatrix(normal).multDirMatrix(unoriented, outDir);
        return { outDir, probability(normal, inDir, outDir) };
    }

    float SpecularBRDF::probability(Vec3f const & normal, Vec3f const & inDir, Vec3f const & outDir) const
    {
        Vec3f reflDir = inDir - 2.0f*(inDir.dotProduct(normal) * normal);
        Vec3f diff = reflDir - outDir;
        if (diff.norm() < 1E-6) return 1.0f;
        else return 0.0f;
    }

    BRDFSample Orca::SpecularBRDF::generateOutSample(Vec3f const & normal, Vec3f const & inDir) const
    {
        return {inDir - 2.0f*(inDir.dotProduct(normal) * normal), 1.0f};
    }

}