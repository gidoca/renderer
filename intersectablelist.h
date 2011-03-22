#ifndef AGGREGATEINTERSECTABLE_H
#define AGGREGATEINTERSECTABLE_H

#include "intersectable.h"

#include <list>
#include <QSharedPointer>

class IntersectableList : public Intersectable
{

  public:
    IntersectableList(std::list< QSharedPointer<Intersectable> > components);

    HitRecord intersect(Ray ray, double from, double to) const;

  private:
    std::list<QSharedPointer<Intersectable> > components;
  
};

#endif
