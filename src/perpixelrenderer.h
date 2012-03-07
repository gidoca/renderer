#ifndef PERPIXELRENDERER_H
#define PERPIXELRENDERER_H

#include "global.h"
#include "renderer.h"

#include <QSize>

#include <vector>
#include <string>

class PerPixelRenderer: public Renderer
{
public:
  void render(const Scene & scene, Film & film, const boost::program_options::variables_map vm);

  static boost::program_options::options_description options();

  static const std::string name;
	
private:
  Integrator * integrator;
};

#endif // PERPIXELRENDERER_H
