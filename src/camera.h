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
    Camera() {}
    Camera(QVector3D cop, QVector3D lookAt, QVector3D up, float fov, QSize resolution) : cop(cop), lookAt(lookAt), up(up), fov(fov), resolution(resolution)
    {
        init();
    }

    QSize getResolution() const
    {
      return resolution;
    }
    QVector3D getCOP() const
    {
        return cop;
    }
    QVector3D getLookAt() const
    {
        return lookAt;
    }
    QVector3D getUp() const
    {
        return up;
    }
    float getFOV() const
    {
        return fov;
    }

    void setResolution(QSize resolution)
    {
        this->resolution = resolution;
        init();
    }
    void setCOP(QVector3D cop)
    {
        this->cop = cop;
        init();
    }
    void setLookAt(QVector3D lookAt)
    {
        this->lookAt = lookAt;
        init();
    }
    void setUp(QVector3D up)
    {
        this->up = up;
        init();
    }
    void setFOV(float fov)
    {
        this->fov = fov;
        init();
    }

    Ray getRay(QPointF point) const;
  private:
    void init();

    float t, b, l, r, aspect;
    QVector3D cop, lookAt, up;
    float fov;
    QSize resolution;
    QMatrix4x4 extrinsicMatrix;
};

#endif
