#pragma once

#include "algebra.h"

namespace Orca
{
    struct BRDFSample
    {
        Vec3f dir;
        float probability;
    };

    class BRDF
    {
    public:
        BRDF();
        ~BRDF();

        virtual float probability(Vec3f const &normal, Vec3f const &inDir, Vec3f const &outDir) const = 0;
        virtual BRDFSample generateOutSample(Vec3f const &normal, Vec3f const &inDir) const = 0;
    };

    class DiffuseBRDF : public BRDF
    {
    public:
        float probability(Vec3f const &normal, Vec3f const &inDir, Vec3f const &outDir) const override;
        BRDFSample generateOutSample(Vec3f const &normal, Vec3f const &inDir) const override;
    };

    class IsotropicDF : public BRDF
    {
    public:
        float probability(Vec3f const &normal, Vec3f const &inDir, Vec3f const &outDir) const override
        {
            return 1;
        }

        BRDFSample generateOutSample(Vec3f const &normal, Vec3f const &inDir) const override
        {
            return {Vec3f(0), 0.0f};
        }
    };

    class SpecularBRDF : public BRDF
    {
    public:
        float probability(Vec3f const &normal, Vec3f const &inDir, Vec3f const &outDir) const override;
        BRDFSample generateOutSample(Vec3f const &normal, Vec3f const &inDir) const override;
    };
}