#include "objreader.h"
#include "directionallight.h"
#include "camera.h"
#include "plane.h"
#include "intersectableinstance.h"
#include "bsp.h"
#include "phongmaterial.h"
#include "arealight.h"

#include <list>
#include <QSize>
#include <QSharedPointer>

Intersectable * getScene(void)
{
  QSharedPointer<Material> material(new PhongMaterial(Spectrum(0, 0, 1), Spectrum(1, 1, 1), 64));

  BSPNode * obj = ObjReader::getMesh("objfiles/teapot.obj", material);
  
  std::list<QSharedPointer<Intersectable> > scene;
  QMatrix4x4 t;
//  t.scale(0.5);
//  t.translate(0.5, 0, 0);
//  scene.push_back(QSharedPointer<Intersectable>(new IntersectableInstance(t, QSharedPointer<Intersectable>(obj))));
  scene.push_back(QSharedPointer<Intersectable>(QSharedPointer<Intersectable>(obj)));
//  scene.push_back(QSharedPointer<Intersectable>(new AxisAlignedBox(QVector3D(-.5, -.5, -.5), QVector3D(.5, .5, .5), material)));
  scene.push_back(QSharedPointer<Intersectable>(new Plane(QVector4D(0, 1, 0, 0), material)));
  
  return new IntersectableList(scene);
}

Camera getCamera(QSize resolution)
{
  return Camera(QVector3D(0, 1, 5), QVector3D(0, 0.5, 0), QVector3D(0, 1, 0), 1, resolution);
}

std::list<QSharedPointer<Light> > getLight(void)
{
  std::list<QSharedPointer<Light> > lights;
//  lights.push_back(QSharedPointer<Light>(new DirectionalLight(QVector3D(4, -1, -2), Spectrum(1, 1, 1))));
  lights.push_back(QSharedPointer<Light>(new AreaLight(QVector3D(-2, 6, -5), QVector3D(0, -8, 0), QVector3D(8, 0, 0), Spectrum(3, 3, 3))));
  return lights;
}
