/**
 * Copyright (C) 2012 Gian Calgeer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
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
    HitRecord(float rayParameter, Ray ray, const Material* material, QVector3D surfaceNormal);
    HitRecord(float rayParameter, Ray ray, const Material * material, QVector3D surfaceNormal, cv::Point2f texcoords);
    
    void transform(QMatrix4x4 matrix);
    
    float getRayParameter() const;
    QVector3D getIntersectingPoint() const;
    const Material & getMaterial() const;
    QVector3D getSurfaceNormal() const;
    Ray getRay() const;
    bool intersects() const;
    cv::Point2f getTexCoords() const;
    
  private:
    const Material* material;
    float rayParameter;
    QVector3D intersectingPoint;
    QVector3D surfaceNormal;
    Ray ray;
    cv::Point2f texcoords;
};

inline HitRecord::HitRecord() : material(DarkMatter::getInstance()), rayParameter(std::numeric_limits< float >::infinity())
{
}


inline HitRecord::HitRecord(float rayParameter, Ray ray, const Material * material, QVector3D surfaceNormal)
  : material(material), rayParameter(rayParameter), intersectingPoint(ray.evaluate(rayParameter)), surfaceNormal(surfaceNormal), ray(ray)
{
}

inline HitRecord::HitRecord(float rayParameter, Ray ray, const Material *material, QVector3D surfaceNormal, cv::Point2f texcoords)
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
