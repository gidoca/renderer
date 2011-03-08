#ifndef HITRECORD_H
#define HITRECORD_H

#include "ray.h"
#include "darkmatter.h"

#include <QVector3D>
#include <QSharedPointer>

class HitRecord
{

  public:
    HitRecord();
    HitRecord(double rayParameter, Ray ray, QSharedPointer<Material> material, QVector3D surfaceNormal);
    
    void transform(QMatrix4x4 matrix);
    
    double getRayParameter() const;
    QVector3D getIntersectingPoint() const;
    const Material & getMaterial() const;
    QVector3D getSurfaceNormal() const;
    Ray getRay() const;
    bool intersects() const;
    
  private:
    QSharedPointer<Material> material;
    double rayParameter;
    QVector3D intersectingPoint;
    QVector3D surfaceNormal;
    Ray ray;
    
};

#endif // HITRECORD_H
