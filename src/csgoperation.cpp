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
#include "csgoperation.h"

#include "axisalignedbox.h"

#include <algorithm>

void CSGOperation::processIntersection(bool & leftInside, bool & rightInside, bool & currentInside, std::list<IntersectionParameter> & result, std::list<IntersectionParameter>::iterator & currentIntersection) const
{
    bool wasInside = isInside(leftInside, rightInside);
    currentInside = !currentInside;
    bool inside = isInside(leftInside, rightInside);
    if (wasInside != inside)
    {
        result.push_back(*currentIntersection);
    }
    currentIntersection++;
}

std::list<IntersectionParameter> CSGOperation::getCSGIntersection( Ray ray ) const
{
    bool leftInside = false, rightInside = false;
    std::list<IntersectionParameter> leftIntersection = left->getCSGIntersection ( ray );
    std::list<IntersectionParameter> rightIntersection = right->getCSGIntersection ( ray );

    std::list<IntersectionParameter> result;

    std::list<IntersectionParameter>::iterator leftIterator = leftIntersection.begin();
    std::list<IntersectionParameter>::iterator rightIterator = rightIntersection.begin();

    while ( leftIterator != leftIntersection.end() || rightIterator != rightIntersection.end() )
    {
        if(leftIterator != leftIntersection.end() && (rightIterator == rightIntersection.end() || leftIterator->t <= rightIterator->t))
        {
            processIntersection(leftInside, rightInside, leftInside, result, leftIterator);
        }
        else
        {
            processIntersection(leftInside, rightInside, rightInside, result, rightIterator);
        }
    }

    return result;
}

const AxisAlignedBox * CSGOperation::createBoundingBox()
{
    const AxisAlignedBox * leftBB = left->boundingBox();
    const AxisAlignedBox * rightBB = right->boundingBox();
    QVector3D min ( std::max ( leftBB->getMin().x(), rightBB->getMin().x() ),
                    std::max ( leftBB->getMin().y(), rightBB->getMin().y() ),
                    std::max ( leftBB->getMin().z(), rightBB->getMin().z() ) );
    QVector3D max ( std::min ( leftBB->getMax().x(), rightBB->getMax().x() ),
                    std::min ( leftBB->getMax().y(), rightBB->getMax().y() ),
                    std::min ( leftBB->getMax().z(), rightBB->getMax().z() ) );
    AxisAlignedBox * result = new AxisAlignedBox ( min, max );
    return result;
}

bool CSGIntersection::isInside(bool leftInside, bool rightInside) const
{
    return leftInside && rightInside;
}

bool CSGUnion::isInside(bool leftInside, bool rightInside) const
{
    return leftInside || rightInside;
}

bool CSGDifference::isInside(bool leftInside, bool rightInside) const
{
    return leftInside && !rightInside;
}

bool CSGXor::isInside(bool leftInside, bool rightInside) const
{
    return (!leftInside && rightInside) || (leftInside && !rightInside);
}
