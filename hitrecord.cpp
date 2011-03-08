#include "hitrecord.h"

#include <limits>

HitRecord::HitRecord() : material(QSharedPointer<Material>(new DarkMatter)), rayParameter(std::numeric_limits< double >::infinity())
{
}


HitRecord::HitRecord(double rayParameter, Ray ray, QSharedPointer < Material > material, QVector3D surfaceNormal)
  : material(material), rayParameter(rayParameter), intersectingPoint(ray.evaluate(rayParameter)), surfaceNormal(surfaceNormal), ray(ray)
{ 
}

double HitRecord::getRayParameter() const
{
  return rayParameter;
}

QVector3D HitRecord::getIntersectingPoint() const
{
  return intersectingPoint;
}

const Material & HitRecord::getMaterial() const
{
  return *material;
}

QVector3D HitRecord::getSurfaceNormal() const
{
  return surfaceNormal;
}

void HitRecord::transform(QMatrix4x4 matrix)
{
  intersectingPoint = matrix.map(intersectingPoint);
}

Ray HitRecord::getRay() const
{
  return ray;
}