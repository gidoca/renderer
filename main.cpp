#include "camera.h"
#include "plane.h"
#include "sphere.h"
#include "triangle.h"
#include "binaryintegrator.h"
#include "intersectableinstance.h"
#include "spectrum.h"
#include "diffusematerial.h"
#include "intersectablelist.h"
#include "directionallight.h"

#include <QSize>
#include <QImage>
#include <list>

int main(int argc, char **argv) {
  QSize resolution(512, 512);
  Camera camera(QVector3D(0, 0, 0), QVector3D(0, 1, 0), QVector3D(0, 0, 1), 1.5, resolution);

  QSharedPointer<DiffuseMaterial> material(new DiffuseMaterial(Spectrum(1, 0, 0)));
  
  //Plane object(QVector4D(0, 0, 1, 1), material);
  Sphere object(QVector3D(0.3, 2, 0), 0.5, material);

  Plane *object1 = new Plane(QVector4D(0, 0, 1, 1), material);
  Sphere *object2 = new Sphere(QVector3D(0.3, 2, 0), 0.5, material);
  Sphere *object3 = new Sphere(QVector3D(0.3, 2, 1), 0.5, material);
  
  std::list< QSharedPointer<Intersectable> > intersectables;
  //intersectables.push_back(QSharedPointer<Intersectable>(object1));
  intersectables.push_back(QSharedPointer<Intersectable>(object2));
  //intersectables.push_back(QSharedPointer<Intersectable>(object3));
  //IntersectableList object(intersectables);
  //Triangle object(QVector3D(.1, 1, .1), QVector3D(.1, 1, 0), QVector3D(0, 1, .1), material);
  /*QMatrix4x4 transform;
  transform.translate(0, 2, 0);
  IntersectableInstance object(transform, sphere);*/
  
  DirectionalLight light(QVector3D(0, 1, 1), Spectrum(0.7, 0.7, 0.7));
  
  QImage image(resolution, QImage::Format_RGB32);
  BinaryIntegrator integrator;
  
  for(int i = 0; i < image.width(); i++)
  {
    for(int j = 0; j < image.height(); j++)
    {
      QPoint point = QPoint(i, j);
      Ray ray = camera.getRay(point);
      HitRecord hitRecord = object.intersect(ray);
      Spectrum irradiance = 255 * integrator.integrate(object, ray, light);
      
      image.setPixel(point, qRgb((int) irradiance.x(), (int) irradiance.y(), (int) irradiance.z()));
    }
  }
  
  image.save("tst.png");
  return 0;
}
