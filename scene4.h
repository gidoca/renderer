#include "objreader.h"
#include "directionallight.h"
#include "camera.h"
#include "diffusematerial.h"
#include "transparentmaterial.h"
#include "plane.h"
#include "intersectableinstance.h"
#include "sphere.h"

#include <list>
#include <QSize>
#include <QSharedPointer>

Intersectable * getScene(void)
{
  QSharedPointer<Material> material(new DiffuseMaterial(Spectrum(0, 0, 1), Spectrum(1, 1, 1), 64));
  QSharedPointer<Material> transparentMaterial(new TransparentMaterial(.8));

  IntersectableList * obj = ObjReader::getMesh("objfiles/teapot.obj", transparentMaterial);
  
  std::list<QSharedPointer<Intersectable> > scene;
  QMatrix4x4 t;
//   t.scale(0.5);
//   t.translate(0.5, 0, 0);
  scene.push_back(QSharedPointer<Intersectable>(new IntersectableInstance(t, QSharedPointer<Intersectable>(obj))));
  scene.push_back(QSharedPointer<Intersectable>(new Plane(QVector4D(0, 1, 0, 0), material)));
  
  QSharedPointer<Intersectable> sphere(new Sphere(QVector3D(0, 1, 2.5), .8, material));
  scene.push_back(sphere);
  
  return new IntersectableList(scene);
}

Camera getCamera(QSize resolution)
{
  return Camera(QVector3D(0, 1, -5), QVector3D(0, 0.5, 0), QVector3D(0, 1, 0), 1, resolution);
}

std::list<QSharedPointer<Light> > getLight(void)
{
  std::list<QSharedPointer<Light> > lights;
  lights.push_back(QSharedPointer<Light>(new DirectionalLight(QVector3D(4, -1, -2), Spectrum(1, 1, 1))));
  return lights;
}