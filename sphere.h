#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "intersectable.h"

#include <QVector3D>

class Sphere : public Intersectable
{
  public:
    Sphere(QVector3D center, double radius);
    virtual bool intersect(Ray ray) const;
    
  private:
    QVector3D center;
    double radius;
};

#endif // SPHERE_H
