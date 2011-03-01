#include "binaryintegrator.h"

#include <QColor>

uint BinaryIntegrator::integrate(const Intersectable& scene, Ray ray)
{
  if(scene.intersect(ray).getIntersects())
  {
    return QColor(Qt::white).rgb();
  }
  else
  {
    return QColor(Qt::black).rgb();
  }
}


