#include "intersectablelist.h"

#include <limits>

IntersectableList::IntersectableList(std::list<QSharedPointer<Intersectable> > components) : components(components)
{

}

HitRecord IntersectableList::intersect(Ray ray, double from, double to) const
{
  HitRecord hit;

  for(std::list< QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
  {
    Intersectable & component = **i;
    HitRecord currentHit = component.intersect(ray, from, (to < hit.getRayParameter() ? to : hit.getRayParameter()));
    if(currentHit.intersects())
    {
      hit = currentHit;
    }
  }
  return hit;
}
