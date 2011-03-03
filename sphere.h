#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "intersectable.h"
#include "diffusematerial.h"

#include <QVector3D>

class Sphere : public Intersectable
{
  public:
    Sphere(QVector3D center, double radius, Material & material);

    HitRecord intersect(Ray ray) const;
    
  private:
    QVector3D center;
    double radius;
    Material & material;
};

#endif // SPHERE_H
