#include "binaryintegrator.h"

#include <QColor>

uint BinaryIntegrator::integrate(const Intersectable& scene, Ray ray)
{
  if(scene.intersect(ray).getIntersects())
  {
    QColor(Qt::white).rgb();
  }
  else
  {
    QColor(Qt::black).rgb();
  }
}


