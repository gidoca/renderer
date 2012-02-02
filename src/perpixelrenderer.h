#ifndef PERPIXELRENDERER_H
#define PERPIXELRENDERER_H

#include "global.h"
#include "renderer.h"

#include <QSize>

#include <vector>

class PerPixelRenderer: public Renderer
{
public:
	PerPixelRenderer(QSize size, Integrator * integrator);
  virtual ~PerPixelRenderer();
	
	void render(const Intersectable & scene, const Camera & camera, std::vector<Light*> lights, Film & film);
	
private:
  Integrator * integrator;
  QSize resolution;
};

#endif // PERPIXELRENDERER_H