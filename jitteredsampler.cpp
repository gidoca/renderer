#include "jitteredsampler.h"

JitteredSampler::JitteredSampler(int width, int height, unsigned int seed) : Sampler(seed)
{
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      QPointF sample(getRandom() * (double) i / width, getRandom() * (double) j / width);
      samples.push_back(sample);
    }
  }
}

std::list<QPointF> JitteredSampler::getSamples()
{
  return samples;
}
