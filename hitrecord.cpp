#include "hitrecord.h"

HitRecord::HitRecord(double rayParameter, Ray ray) : intersects(true), rayParameter(rayParameter), intersectingPoint(ray.evaluate(rayParameter))
{
  
}

HitRecord HitRecord::noIntersection()
{
  HitRecord record;
  record.intersects = false;
  return record;
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

void HitRecord::transform(QMatrix4x4 matrix)
{
  intersectingPoint = matrix.map(intersectingPoint);
}
