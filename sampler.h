#ifndef SAMPLER_H
#define SAMPLER_H

#include <list>

#include <QtCore/QPointF>
#include <QtGui/QVector3D>

class Sample
{
public:
  Sample(QPointF sample) : sample(sample) {}

  QPointF getSample() const;
  QVector3D getCosineWeightedDirection(QVector3D w, double & pdf) const;

private:
  QPointF sample;
};

class Sampler
{
public:
    virtual std::list<Sample> getSamples() = 0;

protected:
    static double getRandom();
};

inline QPointF Sample::getSample() const
{
  return sample;
}

#endif // SAMPLER_H
