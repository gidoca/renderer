#include "hitrecord.h"

HitRecord::HitRecord(double rayParameter) : intersects(true), rayParameter(rayParameter)
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
