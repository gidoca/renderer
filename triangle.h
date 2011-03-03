#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "plane.h"
#include "hitrecord.h"

#include <QVector3D>

class Triangle : public Intersectable
{
  public:
    Triangle(QVector3D p1, QVector3D p2, QVector3D p3, Material & material);
    
    virtual HitRecord intersect(Ray ray) const;
    
  private:
    QVector3D p1, p2, p3;
    const Material & material;
};

#endif // TRIANGLE_H
