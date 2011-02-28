#include "plane.h"

Plane::Plane(QVector4D planeVector) : planeVector(planeVector)
{

}

HitRecord Plane::intersect(Ray ray) const
{
  double d = QVector3D::dotProduct(ray.getDirection(), planeVector.toVector3D());
  if(d > 0)
  {
    double u = QVector4D::dotProduct(QVector4D(ray.getOrigin(), 1), planeVector) / d;
    return HitRecord(u);
  }
  else
  {
    return HitRecord::noIntersection();
  }
}
