#include "intersectableinstance.h"

IntersectableInstance::IntersectableInstance(QMatrix4x4 transform, QSharedPointer<Intersectable> intersectable) : transform(transform), inverseTransform(transform.inverted()), intersectable(intersectable)
{

}

HitRecord IntersectableInstance::intersect(Ray ray) const
{
  Ray transformedRay = ray.transform(inverseTransform);
  HitRecord hitRecord = intersectable->intersect(transformedRay);
  hitRecord.transform(transform);
  return hitRecord;
}

