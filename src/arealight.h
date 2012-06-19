#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "global.h"
#include "light.h"
#include "jitteredsampler.h"

#include <QtGui/QVector3D>
#include <QtCore/QPointF>

class AreaLight : public Light
{
public:
    AreaLight(QVector3D origin, QVector3D uDirection, QVector3D vDirection, cv::Vec3f intensity);

    cv::Vec3f getIntensity(const QVector3D & at, QVector3D &direction, const Intersectable & scene, const Sample &sample) const;
    Ray getRandomRay(const Sample &sample1, const Sample &sample2, float &pdf) const;

private:
    QVector3D getLocation(QPointF p) const;

    QVector3D origin;
    QVector3D uDirection;
    QVector3D vDirection;
    cv::Vec3f intensity;
    QVector3D normal;
};

#endif // AREALIGHT_H
