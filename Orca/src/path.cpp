#include "stdafx.h"

#include "BRDF.h"
#include "path.h"


Orca::Path::Path()
{
}

Orca::Path::~Path()
{
}

void Orca::Path::appendVertex(const PathVertex &v)
{
    vertices.push_back(v);
}

Orca::PathVertex::PathVertex(Vec3f const &pos, Vec3f const &normal, const Vec3f &inDir,
    const Vec3f & outDir, float probability, BRDF const *brdf) :
    pos(pos), normal(normal), inDir(inDir), outDir(outDir), probability(probability), brdf(brdf)
{

}

Orca::PathVertex::~PathVertex()
{

}
