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
#ifndef CSGOPERATION_H
#define CSGOPERATION_H

#include "global.h"
#include "csgobject.h"

class CSGOperation : public CSGObject
{
public:
  CSGOperation(CSGObject* left, CSGObject* right) : left(left), right(right) {}

  std::list<IntersectionParameter> getCSGIntersection(Ray ray) const;

protected:
  const AxisAlignedBox * createBoundingBox();

  virtual bool isInside(bool leftInside, bool rightInside) const = 0;

private:
  void processIntersection(bool & leftInside, bool & rightInside, bool & currentInside, std::list<IntersectionParameter> & result, std::list<IntersectionParameter>::iterator & currentIntersection) const;

  CSGObject *left, *right;
};

#define CSG_OPERATION(name) \
class name : public CSGOperation \
{ \
public: \
    name(CSGObject* left, CSGObject* right) : CSGOperation(left, right) {} \
protected: \
    bool isInside(bool leftInside, bool rightInside) const; \
}

CSG_OPERATION(CSGIntersection);
CSG_OPERATION(CSGUnion);
CSG_OPERATION(CSGDifference);
CSG_OPERATION(CSGXor);

#endif // CSGOPERATION_H
