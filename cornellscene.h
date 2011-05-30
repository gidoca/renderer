#include <QtGui/QVector3D>
#include <QtCore/QSize>

#include <cmath>

#include "camera.h"
#include "pointlight.h"
#include "directionallight.h"
#include "sphere.h"
#include "plane.h"
#include "quad.h"
#include "intersectablelist.h"
#include "intersectableinstance.h"
#include "mirrormaterial.h"
#include "axisalignedbox.h"
#include "bsp.h"
#include "phongmaterial.h"
#include "participatingmaterial.h"
#include "arealight.h"

Camera getCamera(QSize resolution)
{
  // Make camera and film
  QVector3D eye(278, 273, -800);
  QVector3D lookAt(278, 273, -799);
  QVector3D up(0.f,1.f,0.f);
  float fov = M_PI / 5;
  return Camera(eye, lookAt, up, fov, resolution);
}

Intersectable * getScene(void)
{
  std::list< QSharedPointer<Intersectable> > objects;

  Spectrum kd = .4 * Spectrum(1, 1, 1);
  Quad * plane = new Quad(QVector3D(552.8, 0, 0), QVector3D(0, 0, 0), QVector3D(0, 0, 559.2), QVector3D(549.6, 0, 559.2), QSharedPointer<Material>(new PhongMaterial(kd, Spectrum(), 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  plane = new Quad(QVector3D(556, 548.8, 0), QVector3D(556, 548.8, 559.2), QVector3D(0, 548.8, 559.2), QVector3D(0, 548.8, 0), QSharedPointer<Material>(new PhongMaterial(kd, Spectrum(), 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  plane = new Quad(QVector3D(549.6, 0, 559.2), QVector3D(0, 0, 559.2), QVector3D(0, 548.8, 559.2), QVector3D(556, 548.8, 559.2), QSharedPointer<Material>(new PhongMaterial(kd, Spectrum(), 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  kd = .5 * Spectrum(0, 1, 0);
  plane = new Quad(QVector3D(0, 0, 559.2), QVector3D(0, 0, 0), QVector3D(0, 548.8, 0), QVector3D(0, 548.8, 559.2), QSharedPointer<Material>(new PhongMaterial(kd, Spectrum(), 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  kd = .5 * Spectrum(1, 0, 0);
  plane = new Quad(QVector3D(552.8, 0, 0), QVector3D(549.6, 0, 559.2), QVector3D(556.0, 548.8, 559.2), QVector3D(556, 548.8, 0), QSharedPointer<Material>(new PhongMaterial(kd, Spectrum(), 32)));
  objects.push_back(QSharedPointer<Intersectable>(plane));

  kd = Spectrum(.5, .5, .5);
//  AxisAlignedBox * smallBox = new AxisAlignedBox(QVector3D(-82.5, -82.5, -82.5), QVector3D(82.5, 82.5, 82.5), QSharedPointer<Material>(new PhongMaterial(kd, Spectrum(), 32)));
  AxisAlignedBox * smallBox = new AxisAlignedBox(QVector3D(-82.5, -82.5, -82.5), QVector3D(82.5, 82.5, 82.5), QSharedPointer<Material>(new ParticipatingMaterial()));
  QMatrix4x4 transform;
  transform.translate(185, 82.5, 169);
  transform.rotate(-0.29 * 180 / M_PI, 0, 1, 0);
  IntersectableInstance * smallBoxInstance = new IntersectableInstance(transform, QSharedPointer<Intersectable>(smallBox));
  objects.push_back(QSharedPointer<Intersectable>(smallBoxInstance));

  AxisAlignedBox * largeBox = new AxisAlignedBox(QVector3D(-82.5, -165, -82.5), QVector3D(82.5, 165, 82.5), QSharedPointer<Material>(new MirrorMaterial(1)));
//  Sphere * largeBox = new Sphere(QVector3D(), 80, QSharedPointer<Material>(new MirrorMaterial(1)));
  transform.setToIdentity();
  transform.translate(368, 165, 351);
  transform.rotate(-1.27 * 180 / M_PI, 0, 1, 0);
  IntersectableInstance * largeBoxInstance = new IntersectableInstance(transform, QSharedPointer<Intersectable>(largeBox));
  objects.push_back(QSharedPointer<Intersectable>(largeBoxInstance));

  const double s = .6;
  const double t = 1 - s;
  Sphere * smallSphere = new Sphere(QVector3D(t * 185 + s * 219 + 50, s * 548, t * 169 + s * 232), 60, QSharedPointer<Material>(new PhongMaterial(kd, Spectrum(), 32)));
  objects.push_back(QSharedPointer<Intersectable>(smallSphere));

//  return BSPNode::buildTree(new IntersectableList(objects));
  return new IntersectableList(objects);
}

std::list<QSharedPointer<Light> > getLight(void)
{
  std::list<QSharedPointer<Light> > lights;
  lights.push_back(QSharedPointer<Light>(new AreaLight(QVector3D(213, 548, 227), QVector3D(13, 0, 0), QVector3D(0, 0, 10.5), 5000 * Spectrum(1, 0.85, 0.43))));
//  lights.push_back(QSharedPointer<Light>(new AreaLight(QVector3D(213, 548, 227), QVector3D(130, 0, 0), QVector3D(0, 0, 105), 250 * Spectrum(1, 0.85, 0.43))));
//  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(213, 548, 227), 10 * Spectrum(200, 200, 200))));
  return lights;
}
