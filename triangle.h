#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "plane.h"
#include "hitrecord.h"

#include <QVector3D>
#include <QSharedPointer>

class Triangle : public Intersectable
{
  public:
    Triangle(QVector3D p1, QVector3D p2, QVector3D p3, QSharedPointer<Material> material);
    Triangle(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D n1, QVector3D n2, QVector3D n3, QSharedPointer<Material> material);
    
    virtual HitRecord intersect(Ray ray) const;
    
  private:
    QVector3D p1, p2, p3, n1, n2, n3;
    QSharedPointer<Material> material;
};

#endif // TRIANGLE_H
