#include "sampler.h"

#include <cstdlib>
#include <QtGlobal>
#include <QTime>

Sampler::Sampler(unsigned int seed)
{
  qsrand(seed);
}

double Sampler::getRandom()
{
  return (double) qrand() / RAND_MAX;
}
