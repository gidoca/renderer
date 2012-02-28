#ifndef CAMERA_H
#define CAMERA_H

#include "global.h"
#include "ray.h"

#include <QtGui/QVector3D>
#include <QtCore/QPointF>
#include <QtCore/QSize>
#include <QtGui/QMatrix4x4>

class Camera
{
  public:
    Camera(QVector3D cop, QVector3D look_at, QVector3D up, float fov, QSize resolution);

    inline QSize getResolution() const
    {
      return resolution;
    }
    
    Ray getRay(QPointF point) const;
  private:
    float t, b, l, r, aspect;
    QSize resolution;
    QMatrix4x4 extrinsicMatrix;
};

#endif
