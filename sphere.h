#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "intersectable.h"
#include "diffusematerial.h"

#include <QVector3D>

class Sphere : public Intersectable
{
  public:
    Sphere(QVector3D center, double radius, QSharedPointer<Material> material);

    HitRecord intersect(Ray ray, double from, double to) const;
    AxisAlignedBox * boundingBox() const;
    
  private:
    QVector3D center;
    double radius;
    QSharedPointer<Material> material;
};

#endif // SPHERE_H
