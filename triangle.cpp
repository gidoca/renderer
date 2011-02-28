#include "triangle.h"

Triangle::Triangle(QVector3D p1, QVector3D p2, QVector3D p3) : trianglePlane(Plane::fromPoints(p1, p2, p3))
{
  
}

HitRecord Triangle::intersect(Ray ray) const
{
  HitRecord trianglePlaneIntersection = trianglePlane.intersect(ray);
  QVector3D intersectingPoint = ray.evaluate(trianglePlaneIntersection.getRayParameter());
  //TODO fixme
  return trianglePlaneIntersection;
}
