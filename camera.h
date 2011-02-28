#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

#include <QVector3D>
#include <QPoint>
#include <QSize>
#include <QMatrix4x4>

class Camera
{
  public:
    Camera(QVector3D cop, QVector3D look_at, QVector3D up, double fov, QSize resolution);
    
    Ray getRay(QPoint point) const;
  private:
    QVector3D u, v, w, cop;
    double fov, t, b, l, r, aspect;
    QSize resolution;
    QMatrix4x4 extrinsicMatrix;
};

#endif