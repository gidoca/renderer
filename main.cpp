#include <QtCore/QSize>
#include <QtGui/QImage>
#include <QtCore/QSharedPointer>
#include <QtCore/QTime>
#include <QtGui/QLabel>
#include <QtGui/QApplication>

#include <list>
#include <iostream>

#include "scene1.h"
#include "perpixelrenderer.h"

int main(int argc, char **argv) {
  QSize resolution(512, 512);

  QApplication app(argc, argv);

 
  const Intersectable * object = getScene();
  const std::list<QSharedPointer<Light> > light = getLight();
  const Camera camera = getCamera(resolution);
  
	PerPixelRenderer renderer(resolution);
	QImage image = renderer.render(*object, camera, light);
  
  delete object;
  if(!image.save("/tmp/tst.png")) std::cout << "Failed to save file.\n";
  QLabel l;
  l.setPixmap(QPixmap::fromImage(image));
  l.show();
  return app.exec();
}
