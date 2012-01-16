#ifndef PERPIXELRENDERER_H
#define PERPIXELRENDERER_H

#include "renderer.h"

#include <QSize>

#include <vector>

class Integrator;

class PerPixelRenderer: public Renderer
{
public:
	PerPixelRenderer(QSize size, Integrator * integrator);
  virtual ~PerPixelRenderer();
	
	Film render(const Intersectable & scene, const Camera & camera, std::vector<QSharedPointer<Light> > lights);
	
private:
  Integrator * integrator;
  QSize resolution;
};

#endif // PERPIXELRENDERER_H
