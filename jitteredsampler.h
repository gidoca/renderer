#ifndef JITTEREDSAMPLER_H
#define JITTEREDSAMPLER_H

#include "sampler.h"

#include <QPointF>

class JitteredSampler : public Sampler
{
public:
    JitteredSampler(int width, int height, unsigned int seed) : Sampler(seed), width(width), height(height)
    {
    }


    std::list<Sample> getSamples();

private:
    std::list<Sample> samples;
    int width;
    int height;
};

#endif // JITTEREDSAMPLER_H
