#include "csgobject.h"

#include <list>

HitRecord CSGObject::intersect(Ray ray, float from, float to) const
{
  IntersectionParameter parameter = getCSGIntersection(ray);
  std::list<float> intersections = parameter.intersections;
  if(intersections.empty() || from > intersections.back()) return HitRecord();
  for(std::list<float>::iterator i = intersections.begin(); i != intersections.end(); i++)
  {
    if(*i >= from)
    {
      if(*i > to)
      {
        return HitRecord();
      }
      else
      {
        return HitRecord(*i, ray, parameter.material, parameter.normal);
      }
    }
  }
  return HitRecord();
}
