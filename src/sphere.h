#ifndef SPHERE_H
#define SPHERE_H

#include "global.h"
#include "ray.h"
#include "csgobject.h"
#include "diffusematerial.h"

#include <QVector3D>

class Sphere : public CSGObject
{
  public:
    Sphere(QVector3D center, float radius, Material* material) : material(material), center(center), radius(radius)
    {

    }

    IntersectionParameter getCSGIntersection(Ray ray) const;
    AxisAlignedBox * boundingBox() const;
    
  private:
    Material* material;
    QVector3D center;
    float radius;
};

#endif // SPHERE_H
