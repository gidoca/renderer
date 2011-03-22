#include "plane.h"

Plane::Plane(QVector4D planeVector, QSharedPointer<Material> material) : planeVector(planeVector), material(material)
{

}

HitRecord Plane::intersect(Ray ray, double from, double to) const
{
  double d = -QVector3D::dotProduct(ray.getDirection().toVector3D(), planeVector.toVector3D());
  double u = QVector4D::dotProduct(ray.getOrigin(), planeVector);
  double t = u / d;
  if(from < t && t < to)
  {
    return HitRecord(t, ray, material, planeVector.toVector3D());
  }
  else
  {
    return HitRecord();
  }
}
