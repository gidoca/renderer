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
#ifndef AXISALIGNEDBOX_H
#define AXISALIGNEDBOX_H

#include "global.h"
#include "csgobject.h"

#include <limits>

#include <QtGui/QVector3D>

class AxisAlignedBox : public CSGObject
{

public:
  AxisAlignedBox() :
    material(DarkMatter::getInstance()),
    min(QVector3D(std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity())),
    max(QVector3D(-std::numeric_limits< float >::infinity(), -std::numeric_limits< float >::infinity(), -std::numeric_limits< float >::infinity()))
  {

  }


  AxisAlignedBox(QVector3D min, QVector3D max, Material* material = DarkMatter::getInstance()): material(material), min(min), max(max)
  {

  }

  virtual ~AxisAlignedBox() {}

  IntersectionParameter getCSGIntersection(Ray ray) const;

  QVector3D getMin() const;
  QVector3D getMax() const;
    
  void includePoint(QVector3D point);
  void includeOther(const AxisAlignedBox *box);

protected:
  const AxisAlignedBox* createBoundingBox();
  
private:
  Material* material;
  QVector3D min, max;
    
};

#endif // AXISALIGNEDBOX_H
