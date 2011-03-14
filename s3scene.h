#include "directionallight.h"
#include "camera.h"
#include "diffusematerial.h"
#include "intersectablelist.h"
#include "triangle.h"
#include "plane.h"
#include "objreader.h"

#include <list>
#include <QSize>
#include <QSharedPointer>

Intersectable * getScene(void)
{
  QSharedPointer<Material> material(new DiffuseMaterial(Spectrum(0, 0, 1), Spectrum(0, 0, 1), 64));
  QSharedPointer<Material> material2(new DiffuseMaterial(Spectrum(0, 1, 1), Spectrum(0, 1, 1), 64));

  std::list<QSharedPointer<Intersectable> > triangles;
  
  triangles.push_back(QSharedPointer<Intersectable>(new Triangle(QVector3D(-1, 0, -1), QVector3D(1, 0, -1), QVector3D(0, 1, 0), material)));
  triangles.push_back(QSharedPointer<Intersectable>(new Triangle(QVector3D(-1, 0, 1), QVector3D(1, 0, 1), QVector3D(0, 1, 0), material)));
  //triangles.push_back(QSharedPointer<Intersectable>(ObjReader::getMesh("objfiles/tr.obj", material)));
  
  triangles.push_back(QSharedPointer<Intersectable>(new Plane(QVector4D(0, 1, 0, 0), material2)));
  
  return new IntersectableList(triangles);
}

Camera getCamera(QSize resolution)
{
  return Camera(QVector3D(5, 1, 1), QVector3D(0, 0.5, 0), QVector3D(0, 1, 0), 1, resolution);
}

std::list<QSharedPointer<Light> > getLight(void)
{
  std::list<QSharedPointer<Light> > lights;
  lights.push_back(new DirectionalLight(QVector3D(0, -1, -2), Spectrum(1, 1, 1)));
  return lights;
}