#include "objreader.h"
#include "directionallight.h"
#include "camera.h"
#include "diffusematerial.h"
#include "plane.h"
#include "intersectableinstance.h"
#include "bsp.h"
#include "triangle.h"

#include <list>
#include <QSize>
#include <QSharedPointer>

Intersectable * getScene(void)
{
  QSharedPointer<Material> material(new DiffuseMaterial(Spectrum(0, 0, 1), Spectrum(1, 1, 1), 64));
  
  std::list< QSharedPointer<Intersectable> > triangles;
  
  Triangle * t = new Triangle(QVector3D(0, 0, 0), QVector3D(0, 0, 1), QVector3D(0, 1, 0), material);

  triangles.push_back(QSharedPointer<Intersectable>(t));
  Triangle * t2 = new Triangle(QVector3D(1, 0, 0), QVector3D(1, 0, 1), QVector3D(1, 1, 0), material);
  triangles.push_back(QSharedPointer<Intersectable>(t2));
  BSPNode * scene = BSPNode::buildTree(new IntersectableList(triangles));
  
  return scene;
}

Camera getCamera(QSize resolution)
{
  return Camera(QVector3D(0, 1, 5), QVector3D(0, 0.5, 0), QVector3D(0, 1, 0), 1, resolution);
}

std::list<QSharedPointer<Light> > getLight(void)
{
  std::list<QSharedPointer<Light> > lights;
  lights.push_back(QSharedPointer<Light>(new DirectionalLight(QVector3D(4, -1, -2), Spectrum(1, 1, 1))));
  return lights;
}