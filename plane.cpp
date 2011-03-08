#include "plane.h"

Plane::Plane(QVector4D planeVector, QSharedPointer<Material> material) : planeVector(planeVector), material(material)
{

}

HitRecord Plane::intersect(Ray ray) const
{
  double d = -QVector3D::dotProduct(ray.getDirection().toVector3D(), planeVector.toVector3D());
  if(d > 0)
  {
    double u = QVector4D::dotProduct(ray.getOrigin(), planeVector) / d;
    return HitRecord(u, ray, material, planeVector.toVector3D());
  }
  else
  {
    return HitRecord();
  }
}
