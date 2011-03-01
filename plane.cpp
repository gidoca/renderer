#include "plane.h"

Plane::Plane(QVector4D planeVector) : planeVector(planeVector)
{

}

Plane Plane::fromPoints(QVector3D p1, QVector3D p2, QVector3D p3)
{
  QVector3D normal = QVector3D::crossProduct(p3 - p1, p2 - p1).normalized();
  double dist = -QVector3D::dotProduct(normal, p1);
  return Plane(QVector4D(normal, dist));
}


HitRecord Plane::intersect(Ray ray) const
{
  double d = QVector3D::dotProduct(ray.getDirection().toVector3D(), planeVector.toVector3D());
  if(d > 0)
  {
    double u = QVector4D::dotProduct(ray.getOrigin(), planeVector) / d;
    return HitRecord(u, ray);
  }
  else
  {
    return HitRecord::noIntersection();
  }
}
