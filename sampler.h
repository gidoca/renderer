#ifndef SAMPLER_H
#define SAMPLER_H

#include <list>

#include <QPointF>

class Sampler
{
public:
    Sampler(unsigned int seed);

    virtual std::list<QPointF> getSamples() = 0;

protected:
    static double getRandom();
};

#endif // SAMPLER_H
