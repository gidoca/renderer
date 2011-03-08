#include "binaryintegrator.h"

#include "scene1.h"

#include <QSize>
#include <QImage>
#include <list>

int main(int argc, char **argv) {
  QSize resolution(512, 512);

  QImage image(resolution, QImage::Format_RGB32);
  BinaryIntegrator integrator;
  
  Intersectable * object = getScene();
  Light * light = getLight();
  Camera camera = getCamera(resolution);
  
  #pragma omp parallel for
  for(int i = 0; i < image.width(); i++)
  {
    for(int j = 0; j < image.height(); j++)
    {
      QPoint point = QPoint(i, j);
      Ray ray = camera.getRay(point);
      HitRecord hitRecord = object->intersect(ray);
      Spectrum irradiance = 255 * integrator.integrate(*object, ray, *light);
  
      #pragma omp critical
      {
	image.setPixel(point, qRgb((int) irradiance.x(), (int) irradiance.y(), (int) irradiance.z()));
      }
    }
  }
  
  delete light;
  delete object;
  
  image.save("tst.png");
  return 0;
}
