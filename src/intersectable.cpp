#include "intersectable.h"

#include "bvh.h"
#include "intersectablelist.h"

std::vector<Intersectable*> Intersectable::containedIntersectables()
{
    return std::vector<Intersectable*>(1, this);
}

Intersectable* Intersectable::createBVH()
{
    return BVHNode::create(new IntersectableList(containedIntersectables()));
}
