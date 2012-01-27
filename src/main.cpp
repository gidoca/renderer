#include <QtCore>
#include <QtGui/QImage>
#include <QtCore/QSharedPointer>
#include <QtCore/QTime>
#include <QtGui/QLabel>
#include <QtGui/QApplication>

#include <list>
#include <vector>
#include <iostream>

#include "scenes/scene1.h"
#include "perpixelrenderer.h"
#include "unidipathtracingintegrator.h"
#include "film.h"
#include "tonemapper.h"
#include "win.h"

void render(QSize resolution, Film & film, const Intersectable * object, const Camera * camera, std::vector<QSharedPointer<Light> > light)
{
  Renderer * renderer = new PerPixelRenderer(resolution, new UniDiPathTracingIntegrator());
  
  renderer->render(*object, *camera, light, film);
  delete renderer;
}



int main(int argc, char **argv) {
  QSize resolution(512, 512);

  QApplication app(argc, argv);

 
  const Intersectable * object = getScene();
  const std::vector<QSharedPointer<Light> > light = getLight();
  const Camera camera = getCamera(resolution);
  
  Tonemapper tonemapper(resolution);
  Film film(resolution);
  QFuture< void > future = QtConcurrent::run(render, resolution, film, object, &camera, light);
  std::cout << "Foo\n";
  
  Win l(film, future);
  l.show();
  
  return app.exec();
}

