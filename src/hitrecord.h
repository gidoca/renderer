#ifndef HITRECORD_H
#define HITRECORD_H

#include "global.h"
#include "ray.h"
#include "darkmatter.h"

#include <QVector3D>

#include <opencv2/core/core.hpp>

#include <limits>

class HitRecord
{

  public:
    HitRecord();
    HitRecord(float rayParameter, Ray ray, Material* material, QVector3D surfaceNormal);
    HitRecord(float rayParameter, Ray ray, Material * material, QVector3D surfaceNormal, cv::Point2f texcoords);
    
    void transform(QMatrix4x4 matrix);
    
    float getRayParameter() const;
    QVector3D getIntersectingPoint() const;
    const Material & getMaterial() const;
    QVector3D getSurfaceNormal() const;
    Ray getRay() const;
    bool intersects() const;
    cv::Point2f getTexCoords() const;
    
  private:
    Material* material;
    float rayParameter;
    QVector3D intersectingPoint;
    QVector3D surfaceNormal;
    Ray ray;
    cv::Point2f texcoords;
};

inline HitRecord::HitRecord() : material(DarkMatter::getInstance()), rayParameter(std::numeric_limits< float >::infinity())
{
}


inline HitRecord::HitRecord(float rayParameter, Ray ray, Material * material, QVector3D surfaceNormal)
  : material(material), rayParameter(rayParameter), intersectingPoint(ray.evaluate(rayParameter)), surfaceNormal(surfaceNormal), ray(ray)
{
}

inline HitRecord::HitRecord(float rayParameter, Ray ray, Material * material, QVector3D surfaceNormal, cv::Point2f texcoords)
  : material(material), rayParameter(rayParameter), intersectingPoint(ray.evaluate(rayParameter)), surfaceNormal(surfaceNormal), ray(ray), texcoords(texcoords)
{
}

inline void HitRecord::transform(QMatrix4x4 matrix)
{
  intersectingPoint = matrix.map(intersectingPoint);
  ray = ray.transform(matrix);
  surfaceNormal = QMatrix4x4(matrix.normalMatrix()).map(surfaceNormal);
}

inline float HitRecord::getRayParameter() const
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
  return rayParameter < std::numeric_limits< float >::infinity();
}

inline cv::Point2f HitRecord::getTexCoords() const
{
    return texcoords;
}

#endif // HITRECORD_H
