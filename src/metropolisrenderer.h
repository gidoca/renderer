#ifndef METROPOLISRENDERER_H
#define METROPOLISRENDERER_H

#include "global.h"
#include "renderer.h"
#include "film.h"

#include <gsl/gsl_rng.h>

#include <vector>

class MetropolisRenderer : public Renderer
{
public:
  void render(const Scene & scene, Film & film, const boost::program_options::variables_map vm);

  static boost::program_options::options_description options();
  
};

#endif // METROPOLISRENDERER_H
