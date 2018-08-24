#pragma once

#include "algebra.h"

namespace Orca
{
    struct EDFSample
    {
        Vec3f dir;
        Vec3f emission;
    };

    struct BRDFSample
    {
        Vec3f dir;
        Vec3f probability;
    };

    // Emission distribution function
    class EDF
    {
    public:
        virtual Vec3f emission(Vec3f const &normal, Vec3f const &outDir) const = 0;
        virtual EDFSample generateOutSample(Vec3f const &normal) const = 0;
    };

    class IsotropicEDF : public EDF
    {
        Vec3f emitIntensity;

    public:
        IsotropicEDF(Vec3f const &emitIntensity) :
            emitIntensity(emitIntensity)
        { }
        Vec3f emission(Vec3f const &normal, Vec3f const &outDir) const override;

        EDFSample generateOutSample(Vec3f const &normal) const override;
    };

    class BRDF
    {

    public:
        virtual Vec3f probability(Vec3f const &normal, Vec3f const &inDir, Vec3f const &outDir) const = 0;
        virtual BRDFSample generateOutSample(Vec3f const &normal, Vec3f const &inDir) const = 0;
    };

    class DiffuseBRDF : public BRDF
    {
        Vec3f color;

    public:
        DiffuseBRDF(Vec3f const &color) :
            color(color)
        {}
        Vec3f probability(Vec3f const &normal, Vec3f const &inDir, Vec3f const &outDir) const override;
        BRDFSample generateOutSample(Vec3f const &normal, Vec3f const &inDir) const override;
    };
 
    class SpecularBRDF : public BRDF
    {
    public:
        Vec3f probability(Vec3f const &normal, Vec3f const &inDir, Vec3f const &outDir) const override;
        BRDFSample generateOutSample(Vec3f const &normal, Vec3f const &inDir) const override;
    };
}