#include "hitrecord.h"

HitRecord::HitRecord() : rayParameter(0), intersects(false), material(new DarkMatter)
{
}


HitRecord::HitRecord(double rayParameter, Ray ray, QSharedPointer < Material > material
)
  : intersects(true), rayParameter(rayParameter), intersectingPoint(ray.evaluate(rayParameter)), material(material)
{ 
}

bool HitRecord::getIntersects() const
{
  return intersects;
}

double HitRecord::getRayParameter() const
{
  return rayParameter;
}

QVector3D HitRecord::getIntersectingPoint() const
{
  return intersectingPoint;
}

Material & HitRecord::getMaterial() const
{
  return *material;
}

void HitRecord::transform(QMatrix4x4 matrix)
{
  intersectingPoint = matrix.map(intersectingPoint);
}
