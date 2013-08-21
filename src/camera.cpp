/**
 * Copyright (C) 2012 Gian Calgeer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "camera.h"

#include <cmath>

#define PRINT_CAM_ARG(arg, stream) \
    stream << "[";\
    stream << camera.arg().x();\
    stream << " ";\
    stream << camera.arg().y();\
    stream << " ";\
    stream << camera.arg().z();\
    stream << "],"

void Camera::init()
{
  QVector3D w = (cop - lookAt).normalized();
  QVector3D u = QVector3D::normal(up, w);
  QVector3D v = QVector3D::crossProduct(w, u);
  aspect = (float) resolution.width() / resolution.height();
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

std::ostream & operator<<(std::ostream & stream, const Camera & camera)
{
    stream << "camera(";
    PRINT_CAM_ARG(getCOP, stream);
    PRINT_CAM_ARG(getLookAt, stream);
    PRINT_CAM_ARG(getUp, stream);
    stream << camera.getFOV();
    stream << ",";
    stream << camera.getResolution().width();
    stream << ",";
    stream << camera.getResolution().height();
    stream << ")";
    return stream;
}
