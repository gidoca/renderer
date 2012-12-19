#include "intersectable.h"

std::vector<Intersectable*> Intersectable::containedIntersectables()
{
    return std::vector<Intersectable*>(1, this);
}
