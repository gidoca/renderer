#include "ray.h"

Ray::Ray(QVector3D origin, QVector3D direction) : origin(origin), direction(direction.normalized())
{

}

QVector3D Ray::getOrigin() const
{
  return origin;
}

QVector3D Ray::getDirection() const
{
  return direction;
}

QVector3D Ray::evaluate(double u) const
{
  return origin + u * direction;
}

Ray Ray::transform(QMatrix4x4 matrix)
{
  return Ray(matrix.map(origin), matrix.map(direction));
}
