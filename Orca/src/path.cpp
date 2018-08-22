#include "stdafx.h"

#include "BRDF.h"
#include "path.h"


Orca::Path::Path()
{
}

Orca::Path::~Path()
{
}

void Orca::Path::appendNode(const PathNode & n)
{
    nodes.push_back(n);
}

Orca::PathNode::PathNode(Vec3f const &pos, Vec3f const &normal, const Vec3f & outDir, float probability, BRDF const *brdf) :
    pos(pos), normal(normal), outDir(outDir), probability(probability), brdf(brdf)
{

}

Orca::PathNode::~PathNode()
{

}
