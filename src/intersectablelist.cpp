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
#include "intersectablelist.h"
#include "axisalignedbox.h"

#include <limits>

IntersectableList::~IntersectableList()
{
}

HitRecord IntersectableList::intersect(Ray ray) const
{
//  if(!bBox->intersect(ray, from, to).intersects()) return HitRecord();

  HitRecord hit;

  for(auto i = components.begin(); i != components.end(); i++)
  {
    const Intersectable & component = **i;
    HitRecord currentHit = component.intersect(ray);
    if(currentHit.intersects())
    {
        assert(!hit.intersects() || currentHit.getRayParameter() <= hit.getRayParameter());
        ray = Ray(ray.getOrigin(), ray.getDirection(), ray.getFrom(), currentHit.getRayParameter());
        hit = currentHit;
    }
  }
  return hit;
}

const AxisAlignedBox * IntersectableList::createBoundingBox()
{
  AxisAlignedBox *out = new AxisAlignedBox;
  for(auto i = components.begin(); i != components.end(); i++)
  {
    const AxisAlignedBox * boundingBox = (*i)->boundingBox();
    out->includeOther(boundingBox);
  }
  return out;
}


std::vector< Intersectable* > IntersectableList::getComponents() const
{
  return components;
}

std::vector<Intersectable*> IntersectableList::containedIntersectables()
{
    std::vector<Intersectable*> out;
    out.reserve(components.size());
    for(auto& i: components)
    {
        std::vector<Intersectable*> subcomponents = i->containedIntersectables();
        out.insert(out.end(), subcomponents.begin(), subcomponents.end());
    }

    return out;
}
