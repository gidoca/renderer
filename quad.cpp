#include "quad.h"

HitRecord Quad::intersect(Ray ray, float from, float to) const
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
