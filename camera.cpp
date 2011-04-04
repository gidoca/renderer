#include "camera.h"

#include <cmath>

Camera::Camera(QVector3D cop, QVector3D look_at, QVector3D up, double fov, QSize resolution)
{
  this->cop = cop;
  w = (cop - look_at).normalized();
  u = QVector3D::normal(up, w);
  v = QVector3D::crossProduct(w, u);
  this->resolution = resolution;
  this->fov = fov;
  aspect = (double) resolution.width() / resolution.height();
  t = tan(fov / 2);
  b = -t;
  r = aspect * t;
  l = -r;
  extrinsicMatrix.setColumn(0, u.toVector4D());
  extrinsicMatrix.setColumn(1, v.toVector4D());
  extrinsicMatrix.setColumn(2, w.toVector4D());
  extrinsicMatrix.setColumn(3, QVector4D(cop, 1));
}

Ray Camera::getRay(QPointF point) const
{
  QVector4D pixelLocation = QVector4D(l + (r - l) * (point.x() + 0.5) / resolution.width(), 
				      t + (b - t) * (point.y() + 0.5) / resolution.height(), 
				      -1,
				      0
 				    );
  return Ray(QVector4D(0, 0, 0, 1), pixelLocation).transform(extrinsicMatrix);
}
