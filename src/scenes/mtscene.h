#include "../objreader.h"
#include "../pointlight.h"
#include "../camera.h"
#include "../diffusematerial.h"
#include "../binarymaterial.h"
#include "../plane.h"
#include "../intersectablelist.h"
#include "../triangle.h"
#include "../sphere.h"

#include <vector>
#include <QtCore/QSize>
#include <QtCore/QSharedPointer>

Intersectable * getScene(void)
{
//  QSharedPointer<Material> material(new DiffuseMaterial(Spectrum(0, 0, 1), Spectrum(1, 1, 1), 64));
  Material* material(new BinaryMaterial(Spectrum(0, 0, 1)));
  
  std::list< Intersectable* > triangles;
  
  Triangle * t = new Triangle(QVector3D(0, 0, 0), QVector3D(0, 1, 0), QVector3D(0, 0, 1), material);
//  Sphere * t = new Sphere(QVector3D(0, 0, 0), .5, material);
  triangles.push_back(t);
  //Intersectable * t2 = new Sphere(QVector3D(0, 0, 0), 1, material);
  Intersectable * t2 = new Triangle(QVector3D(2, 0, 0), QVector3D(2, 1, 0), QVector3D(2, 0, 1), material);
//  Sphere * t2 = new Sphere(QVector3D(1.1, 0, 0), .5, material);
  triangles.push_back(t2);
//   BSPNode * scene = BSPNode::buildTree(new IntersectableList(triangles));
  IntersectableList * scene = new IntersectableList(triangles);
  
  return scene;
}

Camera getCamera(QSize resolution)
{
  return Camera(QVector3D(2, 0, 2), QVector3D(0, 0, 0), QVector3D(0, 1, 0), 1, resolution);
}

std::vector<Light* > getLight(void)
{
  std::vector<Light* > lights;
  lights.push_back(new PointLight(QVector3D(2, 1, 2), Spectrum(1, 1, 1)));
  lights.push_back(new PointLight(QVector3D(2, 2, 2), Spectrum(1, 1, 1)));
  return lights;
}
