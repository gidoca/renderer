#include "intersectableinstance.h"
#include "axisalignedbox.h"

HitRecord IntersectableInstance::intersect(Ray ray, float from, float to) const
{
  Ray transformedRay = ray.transform(inverseTransform);
  HitRecord hitRecord = intersectable->intersect(transformedRay, from, to);
  hitRecord.transform(transform);
  return hitRecord;
}

AxisAlignedBox * IntersectableInstance::boundingBox() const
{
  AxisAlignedBox * untransformed = intersectable->boundingBox();
  AxisAlignedBox * result = new AxisAlignedBox();
  QVector3D min = transform.map(untransformed->getMin()), max = transform.map(untransformed->getMax());
  result->includePoint(QVector3D(min.x(), min.y(), min.z()));
  result->includePoint(QVector3D(min.x(), min.y(), max.z()));
  result->includePoint(QVector3D(min.x(), max.y(), min.z()));
  result->includePoint(QVector3D(min.x(), max.y(), max.z()));
  result->includePoint(QVector3D(max.x(), min.y(), min.z()));
  result->includePoint(QVector3D(max.x(), min.y(), max.z()));
  result->includePoint(QVector3D(max.x(), max.y(), min.z()));
  result->includePoint(QVector3D(max.x(), max.y(), max.z()));
  delete untransformed;
  return result;
}
