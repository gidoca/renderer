#include <QtGui/QVector3D>
#include <QtCore/QSize>

#include <vector>
#include <cmath>

#include "../camera.h"
#include "../pointlight.h"
#include "../arealight.h"
#include "../sphere.h"
#include "../plane.h"
#include "../axisalignedbox.h"
#include "../intersectablelist.h"
#include "../intersectableinstance.h"
#include "../objreader.h"
#include "../phongmaterial.h"

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
  Spectrum kd(0.8f, 0.8f, 0.8f);
//  BSPNode * mesh = ObjReader::getMesh("objfiles/teapot.obj", QSharedPointer<Material>(new TransparentMaterial(0.9)));
  Intersectable * mesh = ObjReader::getMesh("objfiles/car.obj", QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));

  std::list< QSharedPointer<Intersectable> > objects;

  QMatrix4x4 t;

  // Instance one
//  t.translate(0.f, -0.25f, 0.f);
  t.rotate(30, 0, 1, 0);
  t.scale(0.005f);
  t.translate(-101, -54, 15);
  IntersectableInstance * instance = new IntersectableInstance(t, QSharedPointer<Intersectable>(mesh));
  objects.push_back(QSharedPointer<Intersectable>(instance));

  // Instance two
  t = QMatrix4x4();
  t.translate(0.f, 0.25f, 0.f);
  t.scale(0.25f);
  instance = new IntersectableInstance(t, QSharedPointer<Intersectable>(mesh));
//  objects.push_back(QSharedPointer<Intersectable>(instance));

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
  lights.push_back(QSharedPointer<Light>(new AreaLight(.7 * QVector3D(-0.25, 0.9, -0.25), QVector3D(.5, 0, 0), QVector3D(0, 0, .5), 4 * Spectrum(4, 4, 4))));
//  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(0, 0.8, 0.8), Spectrum(.7, .7, .7))));
//  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(-0.8, 0.2, 1), Spectrum(.5, .5, .5))));
  //return new DirectionalLight(QVector3D(0, -1, -1), Spectrum(1, 1, 1));
  return lights;
}
