#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.h"

#include <QVector3D>

#include <opencv2/core/core.hpp>

class Material
{
  public:
    virtual cv::Vec3f shade(const HitRecord & hit, QVector3D direction) const = 0;
    virtual bool isMirror() const = 0;
    virtual const TransparentMaterial* refractive() const
    {
        return 0;
    }
};

#endif
