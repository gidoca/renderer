#ifndef PLANE_H
#define PLANE_H

#include "intersectable.h"

#include <QVector4D>
#include <QVector3D>

class Plane : public Intersectable
{

  public:
    Plane(QVector4D planeVector);
    
    static Plane fromPoints(QVector3D p1, QVector3D p2, QVector3D p3);
    
    virtual HitRecord intersect(Ray ray) const;
    
  private:
    QVector4D planeVector;
};

#endif // PLANE_H
