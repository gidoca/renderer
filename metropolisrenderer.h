#ifndef METROPOLISRENDERER_H
#define METROPOLISRENDERER_H

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
};

#endif // METROPOLISRENDERER_H
