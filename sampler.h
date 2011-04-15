#ifndef SAMPLER_H
#define SAMPLER_H

#include <list>

#include <QPointF>
#include <QVector3D>

class Sample
{
public:
  Sample(QPointF sample) : sample(sample) {}

  QPointF getSample() const;
  QVector3D getCosineWeightedDirection(QVector3D w) const;

private:
  QPointF sample;
};

class Sampler
{
public:
    Sampler(unsigned int seed);

    virtual std::list<Sample> getSamples() = 0;

protected:
    static double getRandom();
};

#endif // SAMPLER_H
