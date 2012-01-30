#include "../camera.h"
#include "../phongmaterial.h"
#include "../intersectablelist.h"
#include "../triangle.h"
#include "../plane.h"
#include "../objreader.h"
#include "../pointlight.h"

#include <vector>
#include <QtCore/QSize>
#include <QtCore/QSharedPointer>

Intersectable * getScene(void)
{
  Material* material(new PhongMaterial(Spectrum(0, 0, 1), Spectrum(0, 0, 1), 64));
  Material* material2(new PhongMaterial(Spectrum(0, 1, 1), Spectrum(0, 1, 1), 64));

  std::list<Intersectable* > triangles;
  
  triangles.push_back(new Triangle(QVector3D(-1, 0, -1), QVector3D(1, 0, -1), QVector3D(0, 1, 0), material));
  triangles.push_back(new Triangle(QVector3D(-1, 0, 1), QVector3D(1, 0, 1), QVector3D(0, 1, 0), material));
  //triangles.push_back(QSharedPointer<Intersectable>(ObjReader::getMesh("objfiles/tr.obj", material)));
  
  triangles.push_back(new Plane(QVector4D(0, 1, 0, 0), material2));
  
  return new IntersectableList(triangles);
}

Camera getCamera(QSize resolution)
{
  return Camera(QVector3D(5, 1, 1), QVector3D(0, 0.5, 0), QVector3D(0, 1, 0), 1, resolution);
}

std::vector<Light* > getLight(void)
{
  std::vector<Light* > lights;
  lights.push_back(new PointLight(QVector3D(0, 1, 2), Spectrum(1, 1, 1)));
  return lights;
}
