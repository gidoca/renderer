#include "camera.h"
#include "plane.h"
#include "sphere.h"

#include <iostream>

#include <QSize>
#include <QImage>

int main(int argc, char **argv) {
  QSize resolution(64, 64);
  Camera camera(QVector3D(0, 0, 0), QVector3D(0, 1, 0), QVector3D(0, 0, 1), 1.5, resolution);
  //Plane object(QVector4D(0, 0, -1, 1));
  Sphere object(QVector3D(0, 4, 0), 0.5);
  QImage image(resolution, QImage::Format_RGB32);
  for(int i = 0; i < image.width(); i++)
  {
    for(int j = 0; j < image.height(); j++)
    {
      QPoint point = QPoint(i, j);
      Ray ray = camera.getRay(point);
      if(object.intersect(ray).getIntersects())
      {
	image.setPixel(point, qRgb(255, 255, 255));
      }
      else
      {
	image.setPixel(point, qRgb(0, 0, 0));
      }
    }
  }
  image.save("tst.png");
  return 0;
}
