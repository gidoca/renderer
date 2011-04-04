#include "jitteredsampler.h"

JitteredSampler::JitteredSampler(int width, int height, unsigned int seed) : Sampler(seed), width(width), height(height)
{
}

std::list<QPointF> JitteredSampler::getSamples()
{
  std::list<QPointF> samples;
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      QPointF sample((getRandom() + (double) i) / (width + 1), (getRandom() + (double) j) / (width + 1));
      samples.push_back(sample);
    }
  }
  return samples;
}
