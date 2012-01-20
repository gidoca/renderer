#ifndef METROPOLISRENDERER_H
#define METROPOLISRENDERER_H

#include "renderer.h"
#include "film.h"
#include "sampler.h"

class MetropolisRenderer : public Renderer
{
public:
  MetropolisRenderer(QSize size): film(size)
  {
    
  }
  
  Film render(const Intersectable & scene, const Camera & camera, std::vector<QSharedPointer<Light> > lights);
  
private:
  Film film;
};

class MetropolisSample
{
public:
  void largeStep();
  void smallStep();
  
  Sample lightSample1, lightSample2;
  Sample cameraSample;
  Sample lightPathSamples[MAX_DEPTH];
  Sample cameraPathSamples[MAX_DEPTH];
};

#endif // METROPOLISRENDERER_H
