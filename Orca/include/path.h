#pragma once

#include "algebra.h"

#include <vector>

namespace Orca {
    class PathNode
    {
    public:
        Vec3f pos;
        Vec3f normal;
        Vec3f outDir;
        float probability;
        BRDF const *brdf;

    public:
        PathNode(Vec3f const &pos, Vec3f const &normal, const Vec3f & outDir, float probability, BRDF const *brdf);
        ~PathNode();
    };

    class Path
    {
    private:
        std::vector<PathNode> nodes;
    public:
        Path();
        ~Path();
        void appendNode(const PathNode &n);
    };
}