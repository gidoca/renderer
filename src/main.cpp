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

class Scene
{
public:
  Scene(Camera camera): camera(camera) {}

  const Intersectable * object;
  Camera camera;
  vector<Light*> light;
};

void render(Renderer * renderer, Film & film, Scene scene, variables_map vm)
{  
  renderer->render(*scene.object, scene.camera, scene.light, film, vm);
  delete renderer;
}



int main(int argc, char **argv) {
  Renderer * metropolisRenderer = new MetropolisRenderer();
  Renderer * perPixelRenderer = new PerPixelRenderer();

  QApplication app(argc, argv);
  options_description command_line_options;

  options_description generic("Generic options");
  generic.add_options()
      ("help,h", "display the help")
      ("renderer,r", value<string>()->default_value("pathtracing"), "The rendering algorithm to be used (either pathtracing or metropolis)")
      ("width,x", value<int>()->default_value(250), "The width of the output image")
      ("height,y", value<int>()->default_value(250), "The height of the output image");

  command_line_options.add(generic);
  command_line_options.add(metropolisRenderer->options());
  command_line_options.add(perPixelRenderer->options());

  variables_map vm;
  store(parse_command_line(argc, argv, command_line_options), vm);
  notify(vm);

  if(vm.count("help"))
  {
    cout << command_line_options << endl;
    return 0;
  }

  Renderer * renderer;
  if(vm["renderer"].as<string>() == "pathtracing")
  {
    renderer = perPixelRenderer;
  }
  else if(vm["renderer"].as<string>() == "metropolis")
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
  Scene scene(camera);
  scene.object = object;
  scene.light = light;
  QFuture< void > future = QtConcurrent::run(render, renderer, film, scene, vm);
  
  Win l(film, future);
  l.show();
  
  return app.exec();
}

