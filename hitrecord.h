#ifndef HITRECORD_H
#define HITRECORD_H

#include "ray.h"
#include "darkmatter.h"

#include <QVector3D>

class HitRecord
{

  public:
    HitRecord(double rayParameter, Ray ray, const Material & material);
    static HitRecord noIntersection();
    
    void transform(QMatrix4x4 matrix);
    
    double getRayParameter() const;
    QVector3D getIntersectingPoint() const;
    bool getIntersects() const;
    const Material & getMaterial() const;
    
  private:
    double rayParameter;
    QVector3D intersectingPoint;
    const Material & material;
    
    bool intersects;
    
    HitRecord() : rayParameter(0), material(DarkMatter()) {};

};

#endif // HITRECORD_H
