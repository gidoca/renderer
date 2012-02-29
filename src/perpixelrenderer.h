#ifndef PERPIXELRENDERER_H
#define PERPIXELRENDERER_H

#include "global.h"
#include "renderer.h"

#include <QSize>

#include <vector>

class PerPixelRenderer: public Renderer
{
public:
  void render(const Intersectable & scene, const Camera & camera, std::vector<Light*> lights, Film & film, boost::program_options::variables_map vm);

  boost::program_options::options_description options() const;
	
private:
  Integrator * integrator;
};

#endif // PERPIXELRENDERER_H
