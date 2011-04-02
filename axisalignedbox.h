#ifndef AXISALIGNEDBOX_H
#define AXISALIGNEDBOX_H

#include "intersectable.h"

#include <limits>

#include <QVector3D>
#include <QSharedPointer>

class Material;

class IntersectionParameter
{
  public:
    float tmin, tmax;
};

class AxisAlignedBox : public Intersectable
{

  public:
    AxisAlignedBox();
    AxisAlignedBox(QVector3D min, QVector3D max, QSharedPointer<Material> material);
    AxisAlignedBox(const AxisAlignedBox& other);
    
    HitRecord intersect(Ray ray, double from = 0.00005, double to = std::numeric_limits< double >::infinity()) const;
    AxisAlignedBox * boundingBox() const;
    
    IntersectionParameter getIntersectionParameter(Ray ray) const;
    
    QVector3D getMin() const;
    QVector3D getMax() const;
    
    void includePoint(QVector3D point);
  
  private:
    QVector3D min, max;
    QSharedPointer<Material> material;
    
};

#endif // AXISALIGNEDBOX_H
