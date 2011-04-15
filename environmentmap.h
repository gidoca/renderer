#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include "hdrloader.h"
#include "light.h"

class EnvironmentMap : public Light
{
public:
    EnvironmentMap(const char *filename, float coefficient);
    virtual ~EnvironmentMap();

    Spectrum getIntensity(HitRecord & hit, QVector3D & direction, const Intersectable & scene, Sample sample) const;

private:
    HDRLoaderResult data;
    float coefficient;
};

#endif // ENVIRONMENTMAP_H
