#ifndef HITRECORD_H
#define HITRECORD_H

#include "ray.h"
#include "darkmatter.h"

#include <QVector3D>
#include <QSharedPointer>

#include <limits>

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

inline HitRecord::HitRecord() : material(DarkMatter::getInstance()), rayParameter(std::numeric_limits< double >::infinity())
{
}


inline HitRecord::HitRecord(double rayParameter, Ray ray, QSharedPointer < Material > material, QVector3D surfaceNormal)
  : material(material), rayParameter(rayParameter), intersectingPoint(ray.evaluate(rayParameter)), surfaceNormal(surfaceNormal), ray(ray)
{
}

inline void HitRecord::transform(QMatrix4x4 matrix)
{
  intersectingPoint = matrix.map(intersectingPoint);
  ray = ray.transform(matrix);
  surfaceNormal = QMatrix4x4(matrix.normalMatrix()).map(surfaceNormal);
}

inline double HitRecord::getRayParameter() const
{
  return rayParameter;
}

inline QVector3D HitRecord::getIntersectingPoint() const
{
  return intersectingPoint;
}

inline const Material & HitRecord::getMaterial() const
{
  return *material;
}

inline QVector3D HitRecord::getSurfaceNormal() const
{
  return surfaceNormal;
}

inline Ray HitRecord::getRay() const
{
  return ray;
}

inline bool HitRecord::intersects() const
{
  return rayParameter < std::numeric_limits< double >::infinity();
}

#endif // HITRECORD_H
