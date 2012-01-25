#ifndef METROPOLISRENDERER_H
#define METROPOLISRENDERER_H

#include "global.h"
#include "renderer.h"
#include "film.h"

class MetropolisRenderer : public Renderer
{
public:
  MetropolisRenderer(QSize size): film(size)
  {
    
  }
  
  Film render(const Intersectable & scene, const Camera & camera, std::vector<QSharedPointer<Light> > lights);
  
private:
  Film film;

  Path cameraPathFromSample(MetropolisSample sample, const Intersectable & scene, const Camera & camera);
};

class MetropolisSample
{
public:
  void largeStep();
  void smallStep();

  Sample lightSample1[MAX_DEPTH], lightSample2[MAX_DEPTH];
  Sample cameraSample;
  Sample lightPathSamples[MAX_DEPTH];
  Sample cameraPathSamples[MAX_DEPTH];

  int lightIndex[MAX_DEPTH];
};

#endif // METROPOLISRENDERER_H
