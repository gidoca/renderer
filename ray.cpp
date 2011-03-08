#include "ray.h"

Ray::Ray()
{
  init(QVector4D(), QVector4D());
}

Ray::Ray(QVector3D origin, QVector3D direction)
{
  init(origin, QVector4D(direction, 0));
}

Ray::Ray(QVector4D origin, QVector4D direction)
{
  init(origin, direction);
}

void Ray::init(QVector4D origin, QVector4D direction)
{
  this->origin = origin;
  this->direction = direction;
}

QVector4D Ray::getOrigin() const
{
  return origin;
}

QVector4D Ray::getDirection() const
{
  return direction;
}

QVector3D Ray::evaluate(double u) const
{
  return origin.toVector3DAffine() + u * direction.toVector3D();
}

Ray Ray::transform(QMatrix4x4 matrix)
{
  return Ray(matrix.map(origin), matrix.map(direction));
}
