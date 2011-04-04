#ifndef JITTEREDSAMPLER_H
#define JITTEREDSAMPLER_H

#include "sampler.h"

#include <QPointF>

class JitteredSampler : public Sampler
{
public:
    JitteredSampler(int width, int height, unsigned int seed);

    std::list<QPointF> getSamples();

private:
    std::list<QPointF> samples;
};

#endif // JITTEREDSAMPLER_H
