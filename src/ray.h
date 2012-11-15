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

#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>

class Ray
{

  public:
    Ray();
    Ray(QVector3D origin, QVector3D direction);
    Ray(QVector4D origin, QVector4D direction);
    
    QVector3D getOrigin() const;
    QVector3D getDirection() const;
    
    QVector3D evaluate(float u) const;
    
    Ray transform(QMatrix4x4 matrix);

  private:
    void init(QVector4D origin, QVector4D direction);
    
    QVector3D origin, direction;
};

inline QVector3D Ray::getOrigin() const
{
  return origin;
}

inline QVector3D Ray::getDirection() const
{
  return direction;
}

inline Ray::Ray()
{
  init(QVector4D(), QVector4D());
}

inline Ray::Ray(QVector3D origin, QVector3D direction)
{
  init(QVector4D(origin, 1), direction);
}

inline Ray::Ray(QVector4D origin, QVector4D direction)
{
  init(origin, direction);
}

inline void Ray::init(QVector4D origin, QVector4D direction)
{
  this->origin = origin.toVector3DAffine();
  this->direction = direction.toVector3D();
}

inline QVector3D Ray::evaluate(float u) const
{
  return origin + u * direction;
}

inline Ray Ray::transform(QMatrix4x4 matrix)
{
  return Ray(matrix.map(QVector4D(origin, 1)), matrix.map(QVector4D(direction, 0)));
}

#endif // RAY_H
