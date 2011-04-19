#include "quad.h"

HitRecord Quad::intersect(Ray ray, double from, double to) const
{
  HitRecord hit = t1.intersect(ray, from, to);
  if(hit.intersects())
  {
    return hit;
  }
  else
  {
    return t2.intersect(ray, from, to);
  }
}

AxisAlignedBox * Quad::boundingBox() const
{
  return new AxisAlignedBox(bBox);
}
