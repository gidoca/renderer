#include <QtGui/QVector3D>
#include <QtCore/QSize>

#include <list>
#include <cmath>

#include "camera.h"
#include "arealight.h"
#include "conelight.h"
#include "sphere.h"
#include "plane.h"
#include "axisalignedbox.h"
#include "intersectablelist.h"
#include "intersectableinstance.h"
#include "objreader.h"
#include "transparentmaterial.h"
#include "phongmaterial.h"
#include "participatingmaterial.h"
#include "bsp.h"

Camera getCamera(QSize resolution)
{
  // Make camera and film
  QVector3D eye(-370, 18, 125);
  QVector3D lookAt(100, 28, 125);
  QVector3D up(0.f,1.f,0.f);
  float fov = M_PI / 8;
  return Camera(eye, lookAt, up, fov, resolution);
}

Intersectable * getScene(void)
{
  Spectrum kd(0., 0., 1.0);
  QSharedPointer<Material> material(new PhongMaterial(.3 * Spectrum(.1, .3, .8), Spectrum(), 32));

  std::list< QSharedPointer<Intersectable> > objects;

  Intersectable * mesh = ObjReader::getMesh("objfiles/car.obj", material);
//  QMatrix4x4 t;

  // Instance one
//  t.translate(0.f, -0.25f, 0.f);
//  t.rotate(30, 0, 1, 0);
//  t.scale(0.005f);
//  t.translate(-211, -150, 15);
//  IntersectableInstance * instance = new IntersectableInstance(t, QSharedPointer<Intersectable>(mesh));
  objects.push_back(QSharedPointer<Intersectable>(mesh));

  QVector4D normal(0.f, 1, 0.f, 55.0);
  kd = Spectrum(0.f, 0.6f, 0.1f);
  Plane * plane = new Plane(normal, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  /*normal = QVector4D(0.f, 0.f, 1.f, 1.f);
  kd = Spectrum(0.3f, 0.8f, 0.8f);
  plane = new Plane(normal, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));*/

  /*normal = QVector4D(-1.f, 0.f, 0.f, 200.f);
  kd = Spectrum(1.f, 0.f, 0.f);
  plane = new Plane(normal, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));*/

  /*normal = QVector4D(1.f, 0.f, 0.f, 1.f);
  kd = Spectrum(0.f, 0.8f, 0.f);
  plane = new Plane(normal, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  normal = QVector4D(0.f, -1.f, 0.f, 1.f);
  kd = Spectrum(0.8f, 0.8f, 0.8f);
  plane = new Plane(normal, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));*/

  AxisAlignedBox * fog = new AxisAlignedBox(QVector3D(20, -50, 200), QVector3D(180, 100, 500), QSharedPointer<Material>(new ParticipatingMaterial()));
  objects.push_back(QSharedPointer<Intersectable>(fog));

  fog = new AxisAlignedBox(QVector3D(5, -50, -250), QVector3D(20, 100, 200), QSharedPointer<Material>(new ParticipatingMaterial()));
  objects.push_back(QSharedPointer<Intersectable>(fog));

  return new IntersectableList(objects);
}

std::list<QSharedPointer<Light> > getLight(void)
{
  std::list<QSharedPointer<Light> > lights;
  lights.push_back(QSharedPointer<Light>(new AreaLight(QVector3D(-150, 100, -135), QVector3D(400, 0, 0), QVector3D(0, 0, 400), 4 * Spectrum(1, 1, 1))));
  lights.push_back(QSharedPointer<Light>(new ConeLight(QVector3D(50, 0, 200), QVector3D(0, -1, 6), M_PI / 8, 900000 * Spectrum(1, 1, 1))));
  lights.push_back(QSharedPointer<Light>(new ConeLight(QVector3D(150, 0, 200), QVector3D(0, -1, 6), M_PI / 8, 900000 * Spectrum(1, 1, 1))));
  return lights;
}
