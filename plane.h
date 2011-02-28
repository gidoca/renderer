#ifndef PLANE_H
#define PLANE_H

#include "intersectable.h"

#include <QVector4D>
#include <QVector3D>

class Plane : Intersectable
{

  public:
    Plane(QVector4D planeVector);
    
    virtual HitRecord intersect(Ray ray) const;
    
  private:
    QVector4D planeVector;
};

#endif // PLANE_H
