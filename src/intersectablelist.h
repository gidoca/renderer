#ifndef AGGREGATEINTERSECTABLE_H
#define AGGREGATEINTERSECTABLE_H

#include "global.h"
#include "intersectable.h"
#include "axisalignedbox.h"

#include <list>

class IntersectableList : public Intersectable
{

  public:
    IntersectableList(std::list<Intersectable*> components) : components(components)
    {

    }
    
    virtual ~IntersectableList();


    HitRecord intersect(Ray ray, float from, float to) const;
    AxisAlignedBox* boundingBox() const;
    
    std::list< Intersectable* > getComponents() const;

  private:
    std::list<Intersectable* > components;
  
};

#endif
