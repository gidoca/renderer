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
#ifndef VECHELPER_H
#define VECHELPER_H

#include <QtGui/QVector3D>

#include <limits>

inline float get(const QVector3D& v, int i)
{
  switch(i)
  {
    case 0: return v.x();
    case 1: return v.y();
    case 2: return v.z();
    default: return std::numeric_limits<float>::quiet_NaN();
  }
}

inline void set(QVector3D& v, int i, float val)
{
    switch(i)
    {
    case 0: v.setX(val); break;
    case 1: v.setY(val); break;
    case 2: v.setZ(val); break;
    }
}

QVector3D reflect(const QVector3D& v, const QVector3D& normal);

#endif
