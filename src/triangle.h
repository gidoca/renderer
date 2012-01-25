#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "global.h"
#include "plane.h"
#include "hitrecord.h"

#include <QtGui/QVector3D>
#include <QtCore/QSharedPointer>

class Triangle : public Intersectable
{
  public:
    Triangle(QVector3D p1, QVector3D p2, QVector3D p3, QSharedPointer<Material> material) : p1(p1), p2(p2), p3(p3), n1(QVector3D::crossProduct(p1 - p2, p1 - p3)), n2(n1), n3(n1), material(material)
    {

    }

    Triangle(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D n1, QVector3D n2, QVector3D n3, QSharedPointer< Material > material) : p1(p1), p2(p2), p3(p3), n1(n1), n2(n2), n3(n3), material(material)
    {

    }


    HitRecord intersect(Ray ray, float from, float to) const;
    AxisAlignedBox * boundingBox() const;
    
  private:
    QVector3D p1, p2, p3, n1, n2, n3;
    QSharedPointer<Material> material;
};

#endif // TRIANGLE_H
