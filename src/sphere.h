#ifndef SPHERE_H
#define SPHERE_H

#include "global.h"
#include "ray.h"
#include "csgobject.h"
#include "diffusematerial.h"

#include <QtGui/QVector3D>

class Sphere : public CSGObject
{
  public:
    Sphere(QVector3D center, float radius, QSharedPointer<Material> material) : material(material), center(center), radius(radius)
    {

    }

    IntersectionParameter getCSGIntersection(Ray ray) const;
    AxisAlignedBox * boundingBox() const;
    
  private:
    QSharedPointer<Material> material;
    QVector3D center;
    float radius;
};

#endif // SPHERE_H
