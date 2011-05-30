#include "csgobject.h"

#include <list>

HitRecord CSGObject::intersect(Ray ray, double from, double to) const
{
  IntersectionParameter parameter = getCSGIntersection(ray);
  std::list<double> intersections = parameter.intersections;
  if(intersections.empty() || from > intersections.back()) return HitRecord();
  for(std::list<double>::iterator i = intersections.begin(); i != intersections.end(); i++)
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
