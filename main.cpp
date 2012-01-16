#include <QtCore/QSize>
#include <QtGui/QImage>
#include <QtCore/QSharedPointer>
#include <QtCore/QTime>
#include <QtGui/QLabel>
#include <QtGui/QApplication>

#include <list>
#include <vector>
#include <iostream>

#include "scenes/tmscene.h"
#include "perpixelrenderer.h"
#include "unidipathtracingintegrator.h"
#include "film.h"
#include "tonemapper.h"

int main(int argc, char **argv) {
  QSize resolution(512, 512);

  QApplication app(argc, argv);

 
  const Intersectable * object = getScene();
  const std::vector<QSharedPointer<Light> > light = getLight();
  const Camera camera = getCamera(resolution);
  
	Renderer * renderer = new PerPixelRenderer(resolution, new UniDiPathTracingIntegrator());
  Tonemapper tonemapper(resolution);
	Film film = renderer->render(*object, camera, light);
  QImage image = tonemapper.tonemap(film);
  
  delete renderer;
  delete object;
  if(!image.save("/tmp/tst.png")) std::cout << "Failed to save file.\n";
  QLabel l;
  l.setPixmap(QPixmap::fromImage(image));
  l.show();
  return app.exec();
}
