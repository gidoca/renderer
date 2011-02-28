#include "camera.h"
#include "plane.h"
#include "sphere.h"
#include "binaryintegrator.h"

#include <iostream>

#include <QSize>
#include <QImage>

int main(int argc, char **argv) {
  QSize resolution(512, 512);
  Camera camera(QVector3D(0, 0, 0), QVector3D(0, 1, 0), QVector3D(0, 0, 1), 1.5, resolution);
  
  //Plane object(QVector4D(0, 0, -1, 1));
  Sphere object(QVector3D(0, 2, 0), 0.5);
  
  QImage image(resolution, QImage::Format_RGB32);
  BinaryIntegrator integrator;
  
  for(int i = 0; i < image.width(); i++)
  {
    for(int j = 0; j < image.height(); j++)
    {
      QPoint point = QPoint(i, j);
      Ray ray = camera.getRay(point);
      HitRecord hitRecord = object.intersect(ray);
      image.setPixel(point, integrator.integrate(object, ray));
    }
  }
  image.save("tst.png");
  return 0;
}
