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
#ifndef RAY_H
#define RAY_H

#include "global.h"
#include "mathhelper.h"

#include <limits>
#include <cassert>
#include <cmath>

#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>

#define EPSILON 0.001

class Ray
{

  public:
    Ray();
    Ray(QVector3D origin, QVector3D direction, float from = EPSILON, float to = std::numeric_limits<float>::infinity());
    Ray(QVector4D origin, QVector4D direction, float from = EPSILON, float to = std::numeric_limits<float>::infinity());
    
    QVector3D getOrigin() const;
    QVector3D getDirection() const;
    float getFrom() const;
    float getTo() const;

    QVector4D evaluate(float u) const;
    bool inRange(float u) const;
    
    Ray transform(QMatrix4x4 matrix);

  private:
    QVector3D origin, direction;
    float from, to;
};

inline QVector3D Ray::getOrigin() const
{
  return origin;
}

inline QVector3D Ray::getDirection() const
{
  return direction;
}

inline float Ray::getFrom() const
{
    return from;
}

inline float Ray::getTo() const
{
    return to;
}

inline Ray::Ray() : Ray(QVector3D(), QVector3D())
{
}

inline Ray::Ray(QVector3D origin, QVector3D direction, float from, float to) : origin(origin), direction(direction), from(from), to(to)
{
}

inline Ray::Ray(QVector4D origin, QVector4D direction, float from, float to) : origin(origin.toVector3DAffine()), direction(direction.toVector3D()), from(from), to(to)
{
}

inline QVector4D Ray::evaluate(float u) const
{
    if(std::isinf(u))
    {
        return signum(u) * QVector4D(direction.normalized(), 0);
    }
    else
    {
        return QVector4D(origin + u * direction, 1);
    }
}

inline bool Ray::inRange(float u) const
{
    return from <= u && u <= to;
}

inline Ray Ray::transform(QMatrix4x4 matrix)
{
  return Ray(matrix.map(QVector4D(origin, 1)), matrix.map(QVector4D(direction, 0)), from, to);
}

#endif // RAY_H
