#include <QtGui/QVector3D>
#include <QtCore/QSize>

#include <cmath>

#include "../camera.h"
#include "../pointlight.h"
#include "../sphere.h"
#include "../plane.h"
#include "../intersectablelist.h"
#include "../mirrormaterial.h"
#include "../axisalignedbox.h"
#include "../phongmaterial.h"
#include "../arealight.h"
#include "../csgintersection.h"

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
  Spectrum kd(0.4f, 0.4f, 0.4f);
//  QSharedPointer<Material> material(new MirrorMaterial(0.7));
  Material* material(new PhongMaterial(kd, kd, 32));
  Sphere * sphere1 = new Sphere(center, radius, material);
  Sphere * sphere2 = new Sphere(QVector3D(.2, 0, 0), radius, material);
//   AxisAlignedBox * sphere1 = new AxisAlignedBox(QVector3D(-.2, -.2, -.2), QVector3D(.2, .2, .2), material);
//   AxisAlignedBox * sphere2 = new AxisAlignedBox(QVector3D(0, -.2, -.2), QVector3D(.4, .2, .2), material);
  CSGIntersection * sphere = new CSGIntersection(sphere1, sphere2);
  //AxisAlignedBox * box = new AxisAlignedBox(QVector3D(-.2, -.2, -.2), QVector3D(.2, .2, .2), material);
//  AxisAlignedBox * box = sphere->boundingBox();
  std::list< Intersectable* > objects;
  objects.push_back(sphere);

  QVector4D normal(0.f, 1.f, 0.f, 1.f);
  kd = Spectrum(0.f, 0.4f, 0.4f);
  Plane * plane = new Plane(normal, new PhongMaterial(kd, kd, 32));
  objects.push_back(plane);

  normal = QVector4D(0.f, 0.f, 1.f, 1.f);
  kd = Spectrum(0.4f, 0.4f, 0.4f);
  plane = new Plane(normal, new PhongMaterial(kd, kd, 32));
  objects.push_back(plane);

  normal = QVector4D(-1.f, 0.f, 0.f, 1.f);
  kd = Spectrum(.5f, 0.4f, 0.4f);
  plane = new Plane(normal, new PhongMaterial(kd, kd, 32));
  objects.push_back(plane);

  normal = QVector4D(1.f, 0.f, 0.f, 1.f);
  kd = Spectrum(0.f, 0.4f, 0.f);
  plane = new Plane(normal, new PhongMaterial(kd, kd, 32));
  objects.push_back(plane);

  normal = QVector4D(0.f, -1.f, 0.f, 1.f);
  kd = Spectrum(0.4f, 0.4f, 0.4f);
  plane = new Plane(normal, new PhongMaterial(kd, kd, 32));
  objects.push_back(plane);

//  return BSPNode::buildTree(new IntersectableList(objects));
  return new IntersectableList(objects);
}

std::vector<Light* > getLight(void)
{
  std::vector<Light* > lights;
//  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(0, 0.8, 0.8), Spectrum(1, 1, 1))));
//   lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(0.3, 0.6, 0.8), Spectrum(4, 4, 4))));
//   lights.push_back(QSharedPointer<Light>(new AreaLight(.7 * QVector3D(-0.25, 0.9, -0.25), QVector3D(.5, 0, 0), QVector3D(0, 0, .5), 4 * Spectrum(4, 4, 4))));
  lights.push_back(new AreaLight(QVector3D(-0.05, 0.9, -0.05), QVector3D(.1, 0, 0), QVector3D(0, 0, .1), 140 * Spectrum(4, 4, 4)));
  return lights;
}