#include <QVector3D>
#include <QSize>

#include <cmath>

#include <vector>
#include <list>

#include <opencv2/core/core.hpp>

#include "../camera.h"
#include "../pointlight.h"
#include "../conelight.h"
#include "../sphere.h"
#include "../plane.h"
#include "../quad.h"
#include "../intersectablelist.h"
#include "../intersectableinstance.h"
#include "../mirrormaterial.h"
#include "../axisalignedbox.h"
#include "../phongmaterial.h"
#include "../pointlight.h"

Camera getCamera(QSize resolution)
{
  // Make camera and film
  QVector3D eye(278, 273, -500);
  QVector3D lookAt(278, 273, 0);
  QVector3D up(0.f,1.f,0.f);
  float fov = 55. * M_PI / 180.;
  return Camera(eye, lookAt, up, fov, resolution);
}

/*Intersectable * getScene(void)
{
  std::list<Intersectable*> objects;

  Spectrum kd = .5 * Spectrum(1, 1, 1);
  Quad * plane = new Quad(QVector3D(552.8, 0, 0), QVector3D(0, 0, 0), QVector3D(0, 0, 559.2), QVector3D(549.6, 0, 559.2), new PhongMaterial(kd, Spectrum(), 32));
  objects.push_back(plane);

  plane = new Quad(QVector3D(556, 548.8, 0), QVector3D(556, 548.8, 559.2), QVector3D(0, 548.8, 559.2), QVector3D(0, 548.8, 0), new PhongMaterial(kd, Spectrum(), 32));
  objects.push_back(plane);

  plane = new Quad(QVector3D(549.6, 0, 559.2), QVector3D(0, 0, 559.2), QVector3D(0, 548.8, 559.2), QVector3D(556, 548.8, 559.2), new PhongMaterial(kd, Spectrum(), 32));
  objects.push_back(plane);

  plane = new Quad(QVector3D(0, 0, 559.2), QVector3D(0, 0, 0), QVector3D(0, 548.8, 0), QVector3D(0, 548.8, 559.2), new PhongMaterial(kd, Spectrum(), 32));
  objects.push_back(plane);

  kd = Spectrum(.9, .5, .5);
  plane = new Quad(QVector3D(552.8, 0, 0), QVector3D(549.6, 0, 559.2), QVector3D(556.0, 548.8, 559.2), QVector3D(556, 548.8, 0), new PhongMaterial(kd, Spectrum(), 32));
  objects.push_back(plane);

  kd = Spectrum(.3, .5, .3);
  AxisAlignedBox * smallBox = new AxisAlignedBox(QVector3D(-82.5, -82.5, -82.5), QVector3D(82.5, 82.5, 82.5), new PhongMaterial(kd, Spectrum(), 32));
//   AxisAlignedBox * smallBox = new AxisAlignedBox(QVector3D(-82.5, -82.5, -82.5), QVector3D(82.5, 82.5, 82.5), QSharedPointer<Material>(new ParticipatingMaterial()));
  QMatrix4x4 transform;
  transform.translate(185, 83.5, 169);
  transform.rotate(-0.29 * 180 / M_PI, 0, 1, 0);
  IntersectableInstance * smallBoxInstance = new IntersectableInstance(transform, smallBox);
  objects.push_back(smallBoxInstance);

  kd = Spectrum(.5, .2, .2);
//  AxisAlignedBox * largeBox = new AxisAlignedBox(QVector3D(-82.5, -165, -82.5), QVector3D(82.5, 165, 82.5), new PhongMaterial(kd, Spectrum(), 32));
  Sphere * largeBox = new Sphere(QVector3D(), 120, new MirrorMaterial(1));
//  Quad * largeBox = new Quad(QVector3D(82.5, -165, -82.5), QVector3D(82.5, -165, 82.5), QVector3D(82.5, 165, 82.5), QVector3D(82.5, 165, -82.5), new MirrorMaterial(1));
  transform.setToIdentity();
  transform.translate(368, 166, 351);
  transform.rotate(-1.27 * 180 / M_PI, 0, 1, 0);
  IntersectableInstance * largeBoxInstance = new IntersectableInstance(transform, largeBox);
  objects.push_back(largeBoxInstance);

//   const float s = .6;
//   const float t = 1 - s;
//   Sphere * smallSphere = new Sphere(QVector3D(t * 185 + s * 219 + 50, s * 548, t * 169 + s * 232), 60, QSharedPointer<Material>(new PhongMaterial(kd, Spectrum(), 32)));
//   objects.push_back(QSharedPointer<Intersectable>(smallSphere));

//  return BSPNode::buildTree(new IntersectableList(objects));
  return new IntersectableList(objects);
}*/

std::vector<const Light*> getLight(void)
{
  std::vector<const Light*> lights;
//  lights.push_back((new AreaLight(QVector3D(213, 548, 227), QVector3D(13, 0, 0), QVector3D(0, 0, 10.5), 5000 * Spectrum(1, 0.85, 0.43))));
//   lights.push_back(QSharedPointer<Light>(new AreaLight(QVector3D(213, 548, 227), QVector3D(130, 0, 0), QVector3D(0, 0, 105), 250 * Spectrum(1, 0.85, 0.43))));
//   lights.push_back(QSharedPointer<Light>(new AreaLight(QVector3D(368, 166, 351), QVector3D(10, 0, 0), QVector3D(0, 0, 7), 1000 * Spectrum(1, 0.85, 0.43))));
   lights.push_back(new PointLight(QVector3D(278, 478, 279.5), cv::Vec3f(412300, 341100, 298600)));
//   lights.push_back(QSharedPointer<Light>(new ConeLight(QVector3D(213, 548, 227), QVector3D(0, -1, 0), M_PI / 32, 10000 * Spectrum(200, 200, 200))));
  return lights;
}

struct v;
