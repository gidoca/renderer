#ifndef AXISALIGNEDBOX_H
#define AXISALIGNEDBOX_H

#include "global.h"
#include "csgobject.h"

#include <limits>

#include <QtGui/QVector3D>
#include <QtCore/QSharedPointer>

class AxisAlignedBox : public CSGObject
{

public:
  AxisAlignedBox() :
    material(DarkMatter::getInstance()),
    min(QVector3D(std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity())),
    max(QVector3D(-std::numeric_limits< float >::infinity(), -std::numeric_limits< float >::infinity(), -std::numeric_limits< float >::infinity()))
  {

  }


  AxisAlignedBox(QVector3D min, QVector3D max, QSharedPointer<Material> material = DarkMatter::getInstance()): material(material), min(min), max(max)
  {

  }

  AxisAlignedBox * boundingBox() const;
    
  IntersectionParameter getCSGIntersection(Ray ray) const;

  QVector3D getMin() const;
  QVector3D getMax() const;
    
  void includePoint(QVector3D point);
  
private:
  QSharedPointer<Material> material;
  QVector3D min, max;
    
};

#endif // AXISALIGNEDBOX_H
