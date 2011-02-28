#include "plane.h"

Plane::Plane(QVector4D planeVector) : planeVector(planeVector)
{

}

HitRecord Plane::intersect(Ray ray) const
{
  double d = QVector3D::dotProduct(ray.getDirection(), planeVector.toVector3D());
  if(d > 0)
  {
    double u = QVector4D::dotProduct(ray.getOrigin().toVector4D(), planeVector) / d;
    return HitRecord(u);
  }
  else
  {
    return HitRecord::noIntersection();
  }
}
