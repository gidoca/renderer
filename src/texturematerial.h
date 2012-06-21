#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "global.h"
#include "material.h"

#include <string>

#include <opencv2/core/core.hpp>

#include <QVector3D>

class TextureMaterial : public Material
{
public:
    TextureMaterial() {}

    void load(std::string filename);
    cv::Vec3f shade(const HitRecord & hit, QVector3D direction) const;

    bool isMirror() const
    {
        return false;
    }

private:
    cv::Vec3f get(cv::Point2f location) const;

    cv::Mat texture;
};

#endif // TEXTUREMATERIAL_H
