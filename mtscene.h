#include "objreader.h"
#include "directionallight.h"
#include "pointlight.h"
#include "camera.h"
#include "diffusematerial.h"
#include "binarymaterial.h"
#include "plane.h"
#include "intersectablelist.h"
#include "bsp.h"
#include "triangle.h"
#include "sphere.h"

#include <list>
#include <QSize>
#include <QSharedPointer>

Intersectable * getScene(void)
{
//  QSharedPointer<Material> material(new DiffuseMaterial(Spectrum(0, 0, 1), Spectrum(1, 1, 1), 64));
  QSharedPointer<Material> material(new BinaryMaterial(Spectrum(0, 0, 1)));
  
  std::list< QSharedPointer<Intersectable> > triangles;
  
  Triangle * t = new Triangle(QVector3D(0, 0, 0), QVector3D(0, 1, 0), QVector3D(0, 0, 1), material);
//  Sphere * t = new Sphere(QVector3D(0, 0, 0), .5, material);
  triangles.push_back(QSharedPointer<Intersectable>(t));
  //Intersectable * t2 = new Sphere(QVector3D(0, 0, 0), 1, material);
  Intersectable * t2 = new Triangle(QVector3D(2, 0, 0), QVector3D(2, 1, 0), QVector3D(2, 0, 1), material);
//  Sphere * t2 = new Sphere(QVector3D(1.1, 0, 0), .5, material);
  triangles.push_back(QSharedPointer<Intersectable>(t2));
  BSPNode * scene = BSPNode::buildTree(new IntersectableList(triangles));
//  IntersectableList * scene = new IntersectableList(triangles);
  
  return scene;
}

Camera getCamera(QSize resolution)
{
  return Camera(QVector3D(2, 0, 2), QVector3D(0, 0, 0), QVector3D(0, 1, 0), 1, resolution);
}

std::list<QSharedPointer<Light> > getLight(void)
{
  std::list<QSharedPointer<Light> > lights;
  lights.push_back(QSharedPointer<Light>(new DirectionalLight(QVector3D(-2, -1, -2), Spectrum(1, 1, 1))));
  lights.push_back(QSharedPointer<Light>(new PointLight(QVector3D(2, 2, 2), Spectrum(1, 1, 1))));
  return lights;
}
