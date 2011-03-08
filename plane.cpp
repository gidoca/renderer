#include "plane.h"

Plane::Plane(QVector4D planeVector, QSharedPointer<Material> material) : planeVector(planeVector), material(material)
{

}

HitRecord Plane::intersect(Ray ray) const
{
  double d = -QVector3D::dotProduct(ray.getDirection().toVector3D(), planeVector.toVector3D());
  double u = QVector4D::dotProduct(ray.getOrigin(), planeVector);
  if(d > 0 && u > 0)
  {
    return HitRecord(u / d, ray, material, planeVector.toVector3D());
  }
  else
  {
    return HitRecord();
  }
}
