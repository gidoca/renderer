#include "intersectablelist.h"
#include "axisalignedbox.h"

#include <limits>

IntersectableList::~IntersectableList()
{
  delete bBox;
}

HitRecord IntersectableList::intersect(Ray ray, float from, float to) const
{
//  if(!bBox->intersect(ray, from, to).intersects()) return HitRecord();

  HitRecord hit;

  for(std::list< Intersectable* >::const_iterator i = components.begin(); i != components.end(); i++)
  {
    const Intersectable & component = **i;
    HitRecord currentHit = component.intersect(ray, from, (to < hit.getRayParameter() ? to : hit.getRayParameter()));
    if(currentHit.intersects())
    {
      hit = currentHit;
    }
  }
  return hit;
}

AxisAlignedBox * IntersectableList::boundingBox() const
{
  QVector3D min(std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity());
  QVector3D max = -min;
  for(std::list<Intersectable*>::const_iterator i = components.begin(); i != components.end(); i++)
  {
    AxisAlignedBox * boundingBox = (*i)->boundingBox();
    QVector3D currentMin = boundingBox->getMin();
    QVector3D currentMax = boundingBox->getMax();
    delete boundingBox;
    if(currentMin.x() < min.x()) min.setX(currentMin.x());
    if(currentMin.y() < min.y()) min.setY(currentMin.y());   
    if(currentMin.z() < min.z()) min.setZ(currentMin.z());   
    if(currentMax.x() > max.x()) max.setX(currentMax.x());
    if(currentMax.y() > max.y()) max.setY(currentMax.y());   
    if(currentMax.z() > max.z()) max.setZ(currentMax.z());   
  }
  return new AxisAlignedBox(min, max, DarkMatter::getInstance());
}


std::list< Intersectable* > IntersectableList::getComponents() const
{
  return components;
}
