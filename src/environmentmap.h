#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include "light.h"
#include "ray.h"

#include <opencv2/core/core.hpp>

class EnvironmentMap : public Light
{
public:
    EnvironmentMap(cv::Vec3f coefficient);
    virtual ~EnvironmentMap() {}

    bool load(std::string filename);

    cv::Vec3f getIntensity(const HitRecord &hit, QVector3D &direction, const Intersectable &scene, const Sample &sample) const;
    Ray getRandomRay(const Sample &sample1, const Sample &, float &pdf) const;

private:
    cv::Vec3f coefficient;

    cv::Mat image;
};

#endif // ENVIRONMENTMAP_H
