#include "intersectableinstance.h"

IntersectableInstance::IntersectableInstance(QMatrix4x4 transform, QSharedPointer<Intersectable> intersectable) : transform(transform), inverseTransform(transform.inverted()), intersectable(intersectable)
{

}

HitRecord IntersectableInstance::intersect(Ray ray, double from, double to) const
{
  Ray transformedRay = ray.transform(inverseTransform);
  HitRecord hitRecord = intersectable->intersect(transformedRay, from, to);
  hitRecord.transform(transform);
  return hitRecord;
}

