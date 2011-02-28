#include "camera.h"

#include <cmath>

Camera::Camera(QVector3D cop, QVector3D look_at, QVector3D up, double fov, QSize resolution)
{
  this->cop = cop;
  this->w = cop - look_at;
  this->w /= w.length();
  this->u = QVector3D::crossProduct(up, w);
  this->u /= u.length();
  this->v = QVector3D::crossProduct(w, u);
  this->resolution = resolution;
  this->fov = fov;
  this->aspect = (double) resolution.width() / resolution.height();
  this->t = tan(fov / 2);
  this->b = -t;
  this->r = aspect * t;
  this->l = -r;
  this->extrinsicMatrix = QMatrix4x4(u.x(), v.x(), w.x(), cop.x(),
				     u.y(), v.y(), w.y(), cop.y(),
				     u.z(), v.z(), w.z(), cop.z(),
				     0,     0,     0,     1);
}

Ray Camera::getRay(QPoint point) const
{
  QVector3D pixelLocation = QVector3D(l + (r - l) * (point.x() + 0.5) / resolution.width(), 
				      b + (t - b) * (point.y() + 0.5) / resolution.height(), 
				      -1);
  return Ray(extrinsicMatrix.map(QVector3D()), extrinsicMatrix.map(pixelLocation));
}
