#ifndef PLANE_H
#define PLANE_H

#include "intersectable.h"
#include "material.h"

#include <QtGui/QVector4D>
#include <QtGui/QVector3D>
#include <QtCore/QSharedPointer>

class Plane : public Intersectable
{

  public:
    Plane(QVector4D planeVector, QSharedPointer<Material> material) : planeVector(planeVector), material(material)
    {

    }


    HitRecord intersect(Ray ray, float from = EPSILON, float to = std::numeric_limits<float>::infinity()) const;
    AxisAlignedBox * boundingBox() const;
    
  private:
    QVector4D planeVector;
    QSharedPointer<Material> material;
};

#endif // PLANE_H
