#ifndef LIGHT_H
#define LIGHT_H

class HitRecord;
class Sample;
class Ray;
class Intersectable;
class Spectrum;
class QVector3D;

class Light
{

  public:
    virtual Spectrum getIntensity(const QVector3D & at, QVector3D & direction, const Intersectable & scene, const Sample& sample) const = 0;
    virtual Ray getRandomRay(const Sample& sample, double &pdf) const = 0;
};

#endif
