#include "intersectablelist.h"

#include <limits>

IntersectableList::IntersectableList(std::list<QSharedPointer<Intersectable> > components) : components(components)
{

}

HitRecord IntersectableList::intersect (Ray ray) const
{
  double closestParameter = std::numeric_limits<double>::infinity();
  HitRecord hit;

  for(std::list< QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
  {
    Intersectable & component = **i;
    HitRecord currentHit = component.intersect(ray);
    if(currentHit.getRayParameter() < closestParameter)
    {
      closestParameter = currentHit.getRayParameter();
      hit = currentHit;
    }
  }
  return hit;
}
