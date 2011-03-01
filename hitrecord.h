#ifndef HITRECORD_H
#define HITRECORD_H

#include "ray.h"

#include <QVector3D>

class HitRecord
{

  public:
    HitRecord(double rayParameter, Ray ray);
    static HitRecord noIntersection();
    
    void transform(QMatrix4x4 matrix);
    
    double getRayParameter() const;
    QVector3D getIntersectingPoint() const;
    bool getIntersects() const;
    
  private:
    double rayParameter;
    QVector3D intersectingPoint;
    
    bool intersects;
    
    HitRecord() : rayParameter(0) {};

};

#endif // HITRECORD_H
