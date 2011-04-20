#include "csgobject.h"

#include <list>

HitRecord CSGObject::intersect(Ray ray, double from, double to) const
{
  IntersectionParameter parameter = getCSGIntersection(ray);
  std::list<double> intersections = parameter.intersections;
  if(intersections.empty()) return HitRecord();
  bool inside = false;
  if(from < intersections.front())
  {
    if(intersections.front() < to)
    {
      return HitRecord(intersections.front(), ray, parameter.material, parameter.normal);
    }
    else
    {
      return HitRecord();
    }
  }
  if(to > intersections.back()) return HitRecord();
  for(std::list<double>::iterator i = intersections.begin(); i != intersections.end(); i++)
  {
    inside = !inside;
    if(from >= *i)
    {
      if(inside)
      {
        return HitRecord(from, ray, parameter.material, parameter.normal);
      }
      else
      {
        i++;
        if(i == intersections.end() || *i > to)
        {
          return HitRecord();
        }
        else
        {
          return HitRecord(*i, ray, parameter.material, parameter.normal);
        }
      }
    }
  }
  return HitRecord();
}
