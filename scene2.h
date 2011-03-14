#include <QVector3D>
#include <QSize>

#include "list"

#include "camera.h"
#include "pointlight.h"
#include "directionallight.h"
#include "sphere.h"
#include "plane.h"
#include "intersectablelist.h"
#include "intersectableinstance.h"
#include "objreader.h"
#include "mirrormaterial.h"

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
  Spectrum kd(0.8f, 0.8f, 0.8f);
  IntersectableList * mesh = ObjReader::getMesh("objfiles/teapot.obj", QSharedPointer<Material>(new MirrorMaterial()));
//   IntersectableList * mesh = ObjReader::getMesh("objfiles/teapot.obj", QSharedPointer<Material>(new DiffuseMaterial(kd, kd, 32)));

  std::list< QSharedPointer<Intersectable> > objects;

  QMatrix4x4 t;

  // Instance one
  t.translate(0.f, -0.25f, 0.f);
  t.scale(0.25f);
  IntersectableInstance * instance = new IntersectableInstance(t, QSharedPointer<Intersectable>(mesh));
  objects.push_back(QSharedPointer<Intersectable>(instance));

  // Instance two
  t = QMatrix4x4();
  t.translate(0.f, 0.25f, 0.f);
  t.scale(0.25f);
  instance = new IntersectableInstance(t, QSharedPointer<Intersectable>(mesh));
  objects.push_back(QSharedPointer<Intersectable>(instance));

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

std::list<QSharedPointer<Light> > getLight(void)
{
  std::list<QSharedPointer<Light> > lights;
  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(0, 0.8, 0.8), Spectrum(.7, .7, .7))));
  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(-0.8, 0.2, 1), Spectrum(.5, .5, .5))));
  //return new DirectionalLight(QVector3D(0, -1, -1), Spectrum(1, 1, 1));
  return lights;
}
