#pragma once

#include "algebra.h"

namespace Orca
{
    class BRDF
    {
    public:
        BRDF();
        ~BRDF();

        virtual Vec3f generateOutSample(Vec3f const &normal, Vec3f const &inDir) const = 0;
    };

    class DiffuseBRDF : public BRDF
    {
    public:
        Vec3f generateOutSample(Vec3f const &normal, Vec3f const &inDir) const override;
    };

    class SpecularBRDF : public BRDF
    {
    public:
        Vec3f generateOutSample(Vec3f const &normal, Vec3f const &inDir) const override;
    };
}