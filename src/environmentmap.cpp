#include "environmentmap.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "sampler.h"
#include "axisalignedbox.h"
#include "material.h"

#include <cmath>
#include <limits>
#include <iostream>

#include <QtGui/QVector2D>

#include <opencv2/highgui/highgui.hpp>

EnvironmentMap::EnvironmentMap(cv::Vec3f coefficient) : coefficient(coefficient)
{
}

bool EnvironmentMap::load(std::string filename)
{
    image = cv::imread(filename, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_COLOR);
    if(image.data == nullptr)
    {
        std::cerr << "Failed to load environment map " << filename << std::endl;
        return false;
    }
    else
    {
        std::cerr << "Successfully loaded environment map " << filename << std::endl;
        return true;
    }
}

inline float pixelFromNormalizedCoord(float coord, float size)
{
    return (((coord + 1.0) / 2.0) * size - 1);
}

cv::Vec3f EnvironmentMap::getIntensity(const HitRecord &hit, QVector3D &direction, const Intersectable &scene, const Sample &sample) const
{
  QVector3D at = hit.getIntersectingPoint();
  float pdf;
  direction = -sample.getCosineWeightedDirection(hit.getSurfaceNormal(), pdf);
  Ray shadowRay(at, direction, -std::numeric_limits<float>::infinity(), -EPSILON);
  HitRecord shadowHit = scene.intersect(shadowRay);
  if(shadowHit.intersects())
  {
      return cv::Vec3f();
  }
  else
  {
      return 1 / pdf * get(direction);
  }
}

cv::Vec3f EnvironmentMap::get(QVector3D direction) const
{
    QVector2D imageCoords(direction.y(), direction.z());
    imageCoords *= acos(direction.x()) / (imageCoords.length() * M_PI);
    float x = pixelFromNormalizedCoord(imageCoords.x(), image.size().width);
    float y = pixelFromNormalizedCoord(imageCoords.y(), image.size().height);
    return image.at<cv::Vec3f>(x, y).mul(coefficient);
}

Ray EnvironmentMap::getRandomRay(const Sample &sample1, const Sample &, float &pdf) const
{
    pdf = 1;
    return Ray(QVector3D(), sample1.getUniformSphereDirection(), -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
}

HitRecord EnvironmentMap::intersect(Ray ray) const
{
    if(isinf(ray.getTo()))
    {
        return HitRecord(ray.getTo(), ray, this, -ray.getDirection().normalized());
    }
    else
    {
        return HitRecord();
    }
}

AxisAlignedBox* EnvironmentMap::createBoundingBox()
{
    float inf = std::numeric_limits<float>::infinity();
    QVector3D infvec = QVector3D(inf, inf, inf);
    return new AxisAlignedBox(-infvec, infvec);
}

cv::Vec3f EnvironmentMap::emission(const HitRecord & hit) const
{
    assert(&hit.getMaterial() == this);
    return get(hit.getRay().getDirection().normalized());
}
