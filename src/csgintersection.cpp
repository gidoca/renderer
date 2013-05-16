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
#include "csgintersection.h"

#include "axisalignedbox.h"

#include <algorithm>

IntersectionParameter CSGIntersection::getCSGIntersection ( Ray ray ) const
{
    bool leftInside = false, rightInside = false;
    IntersectionParameter leftIntersection = left->getCSGIntersection ( ray );
    IntersectionParameter rightIntersection = right->getCSGIntersection ( ray );

    IntersectionParameter result;

    std::list<float>::iterator leftIterator = leftIntersection.intersections.begin();
    std::list<float>::iterator rightIterator = rightIntersection.intersections.begin();

    while ( leftIterator != leftIntersection.intersections.end() && rightIterator != rightIntersection.intersections.end() )
    {
        if ( *leftIterator <= *rightIterator )
        {
            leftInside = !leftInside;
            if ( leftInside && rightInside )
            {
                result.intersections.push_back ( *leftIterator );
                if ( result.material == 0 )
                {
                    result.material = leftIntersection.material;
                    result.normal = leftIntersection.normal;
                }
            }
            leftIterator++;
        }
        else
        {
            rightInside = !rightInside;
            if ( leftInside && rightInside )
            {
                result.intersections.push_back ( *rightIterator );
                if ( result.material == 0 )
                {
                    result.material = rightIntersection.material;
                    result.normal = rightIntersection.normal;
                }
            }
            rightIterator++;
        }
        if ( leftInside && rightInside )
        {
            result.intersections.push_back ( std::min ( *leftIterator, *rightIterator ) );
        }
    }

    return result;
}

const AxisAlignedBox * CSGIntersection::createBoundingBox()
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

