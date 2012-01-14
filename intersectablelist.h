#ifndef AGGREGATEINTERSECTABLE_H
#define AGGREGATEINTERSECTABLE_H

#include "intersectable.h"
#include "axisalignedbox.h"

#include <list>
#include <QtCore/QSharedPointer>

class IntersectableList : public Intersectable
{

  public:
    IntersectableList(std::list<QSharedPointer<Intersectable> > components) : components(components), bBox(boundingBox())
    {

    }
    
    virtual ~IntersectableList();


    HitRecord intersect(Ray ray, float from, float to) const;
    AxisAlignedBox* boundingBox() const;
    
    std::list< QSharedPointer<Intersectable> > getComponents() const;

  private:
    std::list<QSharedPointer<Intersectable> > components;
    const AxisAlignedBox* bBox;
  
};

#endif
