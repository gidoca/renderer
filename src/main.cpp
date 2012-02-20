#include <QtCore>
#include <QApplication>

#include <list>
#include <vector>
#include <iostream>

#include "scenes/scene1.h"
#include "perpixelrenderer.h"
#include "unidipathtracingintegrator.h"
#include "metropolisrenderer.h"
#include "film.h"
#include "tonemapper.h"
#include "win.h"

void render(QSize resolution, Film & film, const Intersectable * object, const Camera * camera, std::vector<Light*> light)
{
  Renderer * renderer = new PerPixelRenderer(resolution, new UniDiPathTracingIntegrator());
//  Renderer * renderer = new MetropolisRenderer(resolution);
  
  renderer->render(*object, *camera, light, film);
  delete renderer;
}



int main(int argc, char **argv) {
  QSize resolution(512, 512);

  QApplication app(argc, argv);

 
  const Intersectable * object = getScene();
  const std::vector<Light*> light = getLight();
  const Camera camera = getCamera(resolution);
  
  Film film(resolution);
  QFuture< void > future = QtConcurrent::run(render, resolution, film, object, &camera, light);
  
  Win l(film, future);
  l.show();
  
  return app.exec();
}

