#include <QVector3D>
#include <QSize>

#include "list"

#include "camera.h"
#include "pointlight.h"
#include "directionallight.h"
#include "sphere.h"
#include "plane.h"
#include "intersectablelist.h"

Camera getCamera(QSize resolution)
{
  // Make camera and film
  QVector3D eye(0.f,0.f,2.f);
  QVector3D lookAt(0.f,0.f,0.f);
  QVector3D up(0.f,1.f,0.f);
  float fov = 1.05;
  return Camera(eye, lookAt, up, fov, resolution);
}

Intersectable * getScene(void)
{
  QVector3D center(0.f,0.f,0.f);
  float radius = 0.2f;
  Spectrum kd(0.8f, 0.8f, 0.8f);
  Sphere * sphere = new Sphere(center, radius, QSharedPointer<Material>(new DiffuseMaterial(kd, kd, 32)));
  std::list< QSharedPointer<Intersectable> > objects;
  objects.push_back(QSharedPointer<Intersectable>(sphere));

  QVector4D normal(0.f, 1.f, 0.f, 1.f);
  kd = Spectrum(0.f, 0.8f, 0.8f);
  Plane * plane = new Plane(normal, QSharedPointer<Material>(new DiffuseMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  normal = QVector4D(0.f, 0.f, 1.f, 1.f);
  kd = Spectrum(0.3f, 0.8f, 0.8f);
  plane = new Plane(normal, QSharedPointer<Material>(new DiffuseMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  normal = QVector4D(-1.f, 0.f, 0.f, 1.f);
  kd = Spectrum(1.f, 0.8f, 0.8f);
  plane = new Plane(normal, QSharedPointer<Material>(new DiffuseMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  normal = QVector4D(1.f, 0.f, 0.f, 1.f);
  kd = Spectrum(0.f, 0.8f, 0.f);
  plane = new Plane(normal, QSharedPointer<Material>(new DiffuseMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  normal = QVector4D(0.f, -1.f, 0.f, 1.f);
  kd = Spectrum(0.8f, 0.8f, 0.8f);
  plane = new Plane(normal, QSharedPointer<Material>(new DiffuseMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  return new IntersectableList(objects);
}

Light * getLight(void)
{
  return new PointLight(QVector3D(0, 0.8, 0.8), Spectrum(1, 1, 1));
}