#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include "hdrloader.h"
#include "light.h"

class EnvironmentMap : public Light
{
public:
    EnvironmentMap(const char *filename);

    Spectrum getIntensity(HitRecord & hit, QVector3D & direction, const Intersectable & scene, QPointF sample) const;

private:
    HDRLoaderResult data;
};

#endif // ENVIRONMENTMAP_H
