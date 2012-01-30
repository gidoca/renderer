#ifndef PLANE_H
#define PLANE_H

#include "global.h"
#include "intersectable.h"
#include "material.h"

#include <QVector4D>
#include <QVector3D>

class Plane : public Intersectable
{

  public:
    Plane(QVector4D planeVector, Material* material) : planeVector(planeVector), material(material)
    {

    }


    HitRecord intersect(Ray ray, float from = EPSILON, float to = std::numeric_limits<float>::infinity()) const;
    AxisAlignedBox * boundingBox() const;
    
  private:
    QVector4D planeVector;
    Material* material;
};

#endif // PLANE_H
