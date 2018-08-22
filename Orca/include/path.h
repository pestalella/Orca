#pragma once

#include "algebra.h"

#include <vector>

namespace Orca {
    class PathVertex
    {
    public:
        Vec3f pos;
        Vec3f normal;
        Vec3f inDir;
        Vec3f outDir;
        float probability;
        BRDF const *brdf;

    public:
        PathVertex(Vec3f const &pos, Vec3f const &normal, const Vec3f &inDir, 
            const Vec3f & outDir, float probability, BRDF const *brdf);
        ~PathVertex();
    };

    class Path
    {
    public:
        std::vector<PathVertex> vertices;
        Vec3f originalLightIntensity;
    public:
        Path();
        ~Path();
        void appendVertex(const PathVertex &v);
    };
}