#include "jitteredsampler.h"

std::list<Sample> JitteredSampler::getSamples()
{
  std::list<Sample> samples;
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      QPointF sample((getRandom() + (double) i) / width, (getRandom() + (double) j) / height);
      samples.push_back(sample);
    }
  }
  return samples;
}
