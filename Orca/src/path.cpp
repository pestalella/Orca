#include "stdafx.h"

#include "BRDF.h"
#include "path.h"

namespace Orca {
    Path::Path()
    {
    }

    Path::~Path()
    {
    }

    void Path::appendVertex(const PathVertex &v)
    {
        vertices.push_back(v);
    }

    PathVertex::PathVertex(Vec3f const &pos, Vec3f const &normal, const Vec3f &inDir,
        const Vec3f & outDir, Vec3f const &probability, BRDF const *brdf, EDF const *edf) :
        pos(pos), normal(normal), inDir(inDir), outDir(outDir),
        probability(probability), brdf(brdf), edf(edf)
    {

    }

    PathVertex::~PathVertex()
    {

    }
}