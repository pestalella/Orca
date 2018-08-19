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

Orca::PathNode::PathNode(Vec3f const &pos, Vec3f const &normal, BRDF const *brdf) :
    pos(pos), normal(normal), brdf(brdf)
{

}

Orca::PathNode::~PathNode()
{

}
