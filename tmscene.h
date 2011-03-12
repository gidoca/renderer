#include "objreader.h"
#include "directionallight.h"
#include "camera.h"
#include "diffusematerial.h"
#include "plane.h"

#include <list>
#include <QSize>
#include <QSharedPointer>

Intersectable * getScene(void)
{
  QSharedPointer<Material> material(new DiffuseMaterial(Spectrum(0, 0, 1), Spectrum(0, 0, 1), 64));

  IntersectableList * obj = ObjReader::getMesh("objfiles/teapot.obj", material);
  
  std::list<QSharedPointer<Intersectable> > scene;
  scene.push_back(QSharedPointer<Intersectable>(obj));
  scene.push_back(QSharedPointer<Intersectable>(new Plane(QVector4D(0, 1, 0, 0), material)));
  
  return new IntersectableList(scene);
}

Camera getCamera(QSize resolution)
{
  return Camera(QVector3D(0, 1, 5), QVector3D(0, 0.5, 0), QVector3D(0, 1, 0), 1, resolution);
}

Light * getLight(void)
{
  return new DirectionalLight(QVector3D(4, -1, -2), Spectrum(1, 1, 1));
}