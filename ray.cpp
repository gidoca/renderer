#include "ray.h"

Ray::Ray(QVector3D origin, QVector3D direction) : origin(origin), direction(direction)
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

