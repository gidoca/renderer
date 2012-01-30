#ifndef QUAD_H
#define QUAD_H

#include "global.h"
#include "intersectable.h"
#include "triangle.h"
#include "axisalignedbox.h"

class Quad : public Intersectable
{
public:
  Quad(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, Material* mat) : t1(p1, p2, p4, mat), t2(p4, p2, p3, mat)
  {
    bBox.includePoint(p1);
    bBox.includePoint(p2);
    bBox.includePoint(p3);
    bBox.includePoint(p4);
  }

  HitRecord intersect(Ray ray, float from, float to) const;

  AxisAlignedBox * boundingBox() const;

private:
  Triangle t1, t2;
  AxisAlignedBox bBox;
};

#endif // QUAD_H
