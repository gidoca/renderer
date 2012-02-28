#include <QtCore>
#include <QApplication>

#include <list>
#include <vector>
#include <iostream>

#include <boost/program_options.hpp>

#include "scenes/cornellscene.h"
#include "perpixelrenderer.h"
#include "unidipathtracingintegrator.h"
#include "metropolisrenderer.h"
#include "film.h"
#include "tonemapper.h"
#include "win.h"

using namespace std;
using namespace boost::program_options;

void render(Renderer * renderer, Film & film, const Intersectable * object, const Camera * camera, vector<Light*> light)
{  
  renderer->render(*object, *camera, light, film);
  delete renderer;
}



int main(int argc, char **argv) {
  Renderer * metropolisRenderer = new MetropolisRenderer();
  Renderer * perPixelRenderer = new PerPixelRenderer(new UniDiPathTracingIntegrator());

  QApplication app(argc, argv);
  options_description generic("Generic options");
  generic.add_options()
      ("help,h", "display the help")
      ("renderer,r", value<string>()->default_value("perpixelrenderer"), "The rendering algorithm to be used (either perpixelrenderer or metropolisrenderer)")
      ("width,x", value<int>()->default_value(250), "The width of the output image")
      ("height,y", value<int>()->default_value(250), "The height of the output image");

  variables_map vm;
  store(parse_command_line(argc, argv, generic), vm);
  notify(vm);

  if(vm.count("help"))
  {
    cout << generic << endl;
    return 0;
  }

  Renderer * renderer;
  if(vm["renderer"].as<string>() == "perpixelrenderer")
  {
    renderer = perPixelRenderer;
  }
  else if(vm["renderer"].as<string>() == "metropolisrenderer")
  {
    renderer = metropolisRenderer;
  }
  else
  {
    cerr << "Unknown renderer: " << vm["renderer"].as<string>() << endl;
    return -1;
  }

  QSize resolution(vm["width"].as<int>(), vm["height"].as<int>());

  const Intersectable * object = getScene();
  const vector<Light*> light = getLight();
  const Camera camera = getCamera(resolution);
  
  Film film(resolution);
  QFuture< void > future = QtConcurrent::run(render, renderer, film, object, &camera, light);
  
  Win l(film, future);
  l.show();
  
  return app.exec();
}

