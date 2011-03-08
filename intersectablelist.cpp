#include "intersectablelist.h"

#include <limits>

IntersectableList::IntersectableList(std::list<QSharedPointer<Intersectable> > components) : components(components)
{

}

HitRecord IntersectableList::intersect (Ray ray) const
{
  HitRecord hit;

  for(std::list< QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
  {
    Intersectable & component = **i;
    HitRecord currentHit = component.intersect(ray);
    if(currentHit.getRayParameter() < hit.getRayParameter())
    {
      hit = currentHit;
    }
  }
  return hit;
}
