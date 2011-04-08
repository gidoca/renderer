#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

#include <QVector3D>
#include <QPointF>
#include <QSize>
#include <QMatrix4x4>

class Camera
{
  public:
    Camera(QVector3D cop, QVector3D look_at, QVector3D up, double fov, QSize resolution);
    
    Ray getRay(QPointF point) const;
  private:
    double t, b, l, r, aspect;
    QSize resolution;
    QMatrix4x4 extrinsicMatrix;
};

#endif
