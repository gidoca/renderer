#ifndef PLANE_H
#define PLANE_H

#include "intersectable.h"
#include "material.h"

#include <QVector4D>
#include <QVector3D>
#include <QSharedPointer>

class Plane : public Intersectable
{

  public:
    Plane(QVector4D planeVector, QSharedPointer<Material> material) : planeVector(planeVector), material(material)
    {

    }


    HitRecord intersect(Ray ray, double from = EPSILON, double to = std::numeric_limits<double>::infinity()) const;
    AxisAlignedBox * boundingBox() const;
    
  private:
    QVector4D planeVector;
    QSharedPointer<Material> material;
};

#endif // PLANE_H
