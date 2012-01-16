#include "../camera.h"
#include "../sphere.h"
#include "../pointlight.h"
#include "../phongmaterial.h"

#include <QtCore/QSize>

#include <cmath>

Camera getCamera(QSize resolution)
{
  // Make camera and film
  QVector3D eye(0.f,0.f,2.f);
  QVector3D lookAt(0.f,0.f,0.f);
  QVector3D up(0.f,1.f,0.f);
  float fov = M_PI / 3;
  return Camera(eye, lookAt, up, fov, resolution);
}

Intersectable * getScene(void)
{
  return new Sphere(QVector3D(), 1, QSharedPointer<Material>(new PhongMaterial(Spectrum(1, 0, 0), Spectrum(1.4, 0, 0), 32)));
}

std::vector<QSharedPointer<Light> > getLight(void)
{
  std::vector<QSharedPointer<Light> > lights;
  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(2, 2, 2), Spectrum(4, 4, 4))));
  return lights;
}
