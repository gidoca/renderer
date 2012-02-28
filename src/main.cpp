#include <QtCore>
#include <QApplication>

#include <list>
#include <vector>
#include <iostream>

#include <tclap/CmdLine.h>

#include "scenes/cornellscene.h"
#include "perpixelrenderer.h"
#include "unidipathtracingintegrator.h"
#include "metropolisrenderer.h"
#include "film.h"
#include "tonemapper.h"
#include "win.h"

using namespace std;
using namespace TCLAP;

void render(Renderer * renderer, Film & film, const Intersectable * object, const Camera * camera, vector<Light*> light)
{  
  renderer->render(*object, *camera, light, film);
  delete renderer;
}



int main(int argc, char **argv) {
  Renderer * renderer;

  QApplication app(argc, argv);
  CmdLine cmd("A ray tracing renderer", ' ', "0.0");

  ValueArg<string> rendererArg("r", "renderer", "The renderer to be used", false, "perpixelrenderer", "name");
  cmd.add(rendererArg);

  ValueArg<int> widthArg("x", "width", "The width of the output image", false, 250, "width");
  cmd.add(widthArg);

  ValueArg<int> heightArg("y", "height", "The height of the output image", false, 250, "height");
  cmd.add(heightArg);

  cmd.parse(argc, argv);

  QSize resolution(widthArg.getValue(), heightArg.getValue());

  if(rendererArg.getValue() == "perpixelrenderer")
  {
    renderer = new PerPixelRenderer(resolution, new UniDiPathTracingIntegrator());
  }
  else if(rendererArg.getValue() == "metropolisrenderer")
  {
    renderer = new MetropolisRenderer(resolution);
  }
  else
  {
    cerr << "Unknown renderer: " << rendererArg.getValue() << endl;
    return -1;
  }

  const Intersectable * object = getScene();
  const vector<Light*> light = getLight();
  const Camera camera = getCamera(resolution);
  
  Film film(resolution);
  QFuture< void > future = QtConcurrent::run(render, renderer, film, object, &camera, light);
  
  Win l(film, future);
  l.show();
  
  return app.exec();
}

