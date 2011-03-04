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
    HitRecord(double rayParameter, Ray ray, QSharedPointer<Material> material);
    
    void transform(QMatrix4x4 matrix);
    
    double getRayParameter() const;
    QVector3D getIntersectingPoint() const;
    bool getIntersects() const;
    Material & getMaterial() const;
    
  private:
    double rayParameter;
    QVector3D intersectingPoint;
    QSharedPointer<Material> material;
    
    bool intersects;

};

#endif // HITRECORD_H
