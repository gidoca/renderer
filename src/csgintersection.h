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
#ifndef CSGINTERSECTION_H
#define CSGINTERSECTION_H

#include "global.h"
#include "csgobject.h"

class CSGIntersection : public CSGObject
{
public:
  CSGIntersection(CSGObject* left, CSGObject* right) : left(left), right(right) {}

  std::list<IntersectionParameter> getCSGIntersection(Ray ray) const;

protected:
  const AxisAlignedBox * createBoundingBox();

  bool isInside(bool leftInside, bool rightInside) const;

private:
  void processIntersection(bool & leftInside, bool & rightInside, bool & currentInside, std::list<IntersectionParameter> & result, std::list<IntersectionParameter>::iterator & currentIntersection) const;

  CSGObject *left, *right;
};

#endif // CSGINTERSECTION_H
