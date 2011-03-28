#include "intersectablelist.h"
#include "axisalignedbox.h"

#include <limits>

IntersectableList::IntersectableList(std::list<QSharedPointer<Intersectable> > components) : components(components)
{

}

HitRecord IntersectableList::intersect(Ray ray, double from, double to) const
{
  HitRecord hit;

  for(std::list< QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
  {
    Intersectable & component = **i;
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
  QVector3D min(std::numeric_limits< double >::infinity(), std::numeric_limits< double >::infinity(), std::numeric_limits< double >::infinity());
  QVector3D max = -min;
  for(std::list<QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
  {
    QVector3D currentMin = (*i)->boundingBox()->getMin();
    QVector3D currentMax = (*i)->boundingBox()->getMax();
    if(currentMin.x() < min.x()) min.setX(currentMin.x());
    if(currentMin.y() < min.y()) min.setY(currentMin.y());   
    if(currentMin.z() < min.z()) min.setZ(currentMin.z());   
    if(currentMax.x() > max.x()) max.setX(currentMax.x());
    if(currentMax.y() > max.y()) max.setY(currentMax.y());   
    if(currentMax.z() > max.z()) max.setZ(currentMax.z());   
  }
  return new AxisAlignedBox(min, max, QSharedPointer<Material>(new DarkMatter()));
}


std::list< QSharedPointer< Intersectable >, std::allocator< QSharedPointer< Intersectable > > > IntersectableList::getComponents() const
{
  return components;
}
