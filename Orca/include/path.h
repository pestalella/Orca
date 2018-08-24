#pragma once

#include "algebra.h"

#include <vector>

class BRDF;
class EDF;

namespace Orca {
    class PathVertex
    {
    public:
        Vec3f pos;
        Vec3f normal;
        Vec3f inDir;
        Vec3f outDir;
        Vec3f probability;
        BRDF const *brdf;
        EDF const *edf;

    public:
        PathVertex(Vec3f const &pos, Vec3f const &normal, const Vec3f &inDir, 
            const Vec3f & outDir, Vec3f const &probability, BRDF const *brdf, EDF const *edf);
        ~PathVertex();
    };

    class Path
    {
    public:
        std::vector<PathVertex> vertices;
    public:
        Path();
        ~Path();
        void appendVertex(const PathVertex &v);
    };
}