#include "ray.h"

Ray::Ray(QVector3D origin, QVector3D direction) : origin(origin), direction(direction)
{

}

const QVector3D Ray::getOrigin()
{
  return origin;
}

const QVector3D Ray::getDirection()
{
  return direction;
}

