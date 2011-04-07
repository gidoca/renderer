#include <QVector3D>
#include <QSize>

#include <cmath>

#include "camera.h"
#include "pointlight.h"
#include "directionallight.h"
#include "sphere.h"
#include "plane.h"
#include "intersectablelist.h"
#include "mirrormaterial.h"
#include "axisalignedbox.h"
#include "bsp.h"
#include "phongmaterial.h"
#include "arealight.h"
#include "environmentmap.h"

Camera getCamera(QSize resolution)
{
  // Make camera and film
  QVector3D eye(0.f,0.f,2.f);
  QVector3D lookAt(0.f,0.f,0.f);
  QVector3D up(0.f,-1.f,0.f);
  float fov = M_PI / 3;
  return Camera(eye, lookAt, up, fov, resolution);
}

Intersectable * getScene(void)
{
  QVector3D center(0.f,0.f,0.f);
  float radius = 0.6f;
  Spectrum kd(0.8f, 0.8f, 0.8f);
  QSharedPointer<Material> material(QSharedPointer<Material>(new PhongMaterial(Spectrum(), kd, 100)));
//   Sphere * sphere = new Sphere(center, radius, QSharedPointer<Material>(new PhongMaterial(kd, kd, 32)));
  Sphere * sphere = new Sphere(center, radius, material);
  //AxisAlignedBox * box = new AxisAlignedBox(QVector3D(-.2, -.2, -.2), QVector3D(.2, .2, .2), material);
  AxisAlignedBox * box = sphere->boundingBox();
  std::list< QSharedPointer<Intersectable> > objects;
  objects.push_back(QSharedPointer<Intersectable>(sphere));

  /*QVector4D normal(0.f, 1.f, 0.f, 1.f);
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
  objects.push_back(QSharedPointer<Intersectable>(plane));*/

//  return BSPNode::buildTree(new IntersectableList(objects));
  return new IntersectableList(objects);
}

std::list<QSharedPointer<Light> > getLight(void)
{
  std::list<QSharedPointer<Light> > lights;
//  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(0, 0.8, 0.8), Spectrum(1, 1, 1))));
  //lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(0.3, 0.6, 0.8), Spectrum(1, 1, 1))));
//  lights.push_back(QSharedPointer<Light>(new AreaLight(QVector3D(0.3, 0.6, 0.8), QVector3D(1, 0, 0), QVector3D(0, 0, 1), Spectrum(4, 4, 4))));
  lights.push_back(QSharedPointer<Light>(new EnvironmentMap("environmentmaps/campus_probe.hdr", .5)));
  return lights;
}
