#include <QtGui/QVector3D>
#include <QtCore/QSize>

#include <cmath>

#include "../camera.h"
#include "../sphere.h"
#include "../plane.h"
#include "../intersectablelist.h"
#include "../phongmaterial.h"
#include "../pointlight.h"
#include "../mirrormaterial.h"

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
  QVector3D center(0.f,0.f,0.f);
  float radius = 0.2f;
  Spectrum kd(0.8f, 0.f, 0.f);
  Sphere * sphere = new Sphere(center, radius, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
//   Sphere * sphere = new Sphere(center, radius, QSharedPointer<Material>(new TransparentMaterial(1.33)));
  std::list< QSharedPointer<Intersectable> > objects;
  objects.push_back(QSharedPointer<Intersectable>(sphere));

  center = QVector3D(0.4f,0.f,-0.3f);
  radius = 0.3f;
  Sphere * msphere = new Sphere(center, radius, QSharedPointer<Material>(new MirrorMaterial(0.85)));
  objects.push_back(QSharedPointer<Intersectable>(msphere));

  QVector4D normal(0.f, 1.f, 0.f, 1.f);
  kd = Spectrum(0.f, 0.8f, 0.8f);
  Plane * plane = new Plane(normal, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  normal = QVector4D(0.f, 0.f, 1.f, 1.f);
  kd = Spectrum(0.3f, 0.8f, 0.8f);
  plane = new Plane(normal, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  normal = QVector4D(-1.f, 0.f, 0.f, 1.f);
  kd = Spectrum(1.f, 0.8f, 0.8f);
  plane = new Plane(normal, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  normal = QVector4D(1.f, 0.f, 0.f, 1.f);
  kd = Spectrum(0.f, 0.8f, 0.f);
  plane = new Plane(normal, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  normal = QVector4D(0.f, -1.f, 0.f, 1.f);
  kd = Spectrum(0.8f, 0.8f, 0.8f);
  plane = new Plane(normal, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  return new IntersectableList(objects);
}

std::vector<QSharedPointer<Light> > getLight(void)
{
  std::vector<QSharedPointer<Light> > lights;
  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(0, 0.8, 0.8), Spectrum(1, 1, 1))));
  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(0, 0.6, 0.8), Spectrum(1, 1, 1))));
  return lights;
}
