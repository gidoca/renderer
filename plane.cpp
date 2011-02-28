#include "plane.h"

Plane::Plane(QVector4D planeVector) : planeVector(planeVector)
{

}

bool Plane::intersect(Ray ray) const
{
  double d = QVector3D::dotProduct(ray.getDirection(), planeVector.toVector3D());
  return d > 0;
  //double u = QVector4D::dotProduct(ray.getOrigin().toVector4D(), planeVector) / d;
}
