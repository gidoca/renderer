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
#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include "global.h"
#include "ray.h"
#include "hitrecord.h"

#include <QtGui/QVector3D>

#include <limits>
#include <vector>

class AxisAlignedBox;

class Intersectable
{
  public:
    Intersectable() : bb(nullptr) {}
    virtual ~Intersectable() {}
    virtual HitRecord intersect(Ray ray) const = 0;
    inline const AxisAlignedBox * boundingBox()
    {
        if(bb == nullptr) bb = createBoundingBox();
        return bb;
    }

    virtual std::vector<Intersectable*> containedIntersectables();
    virtual Intersectable* createBVH();

  protected:
    virtual const AxisAlignedBox * createBoundingBox() = 0;
    const AxisAlignedBox * bb;
};

#endif // INTERSECTABLE_H
