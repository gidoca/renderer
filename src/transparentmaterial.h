#ifndef TRANSPARENTMATERIAL_H
#define TRANSPARENTMATERIAL_H

#include "material.h"

#include <QVector3D>

class TransparentMaterial : public Material
{
public:
    TransparentMaterial(float refractionCoeff);

    virtual bool isMirror() const
    {
        return false;
    }

    virtual const TransparentMaterial* refractive() const
    {
        return this;
    }

    virtual cv::Vec3f shade(const HitRecord &, QVector3D) const
    {
        return cv::Vec3f();
    }

    QVector3D outDirection(QVector3D inDirection, QVector3D normal) const;

private:
    float refractionCoeff;
};

#endif // TRANSPARENTMATERIAL_H
