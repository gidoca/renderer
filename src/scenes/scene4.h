#include "../objreader.h"
#include "../pointlight.h"
#include "../camera.h"
#include "../phongmaterial.h"
#include "../plane.h"
#include "../intersectableinstance.h"
#include "../sphere.h"
#include "../intersectablelist.h"

#include <vector>
#include <QtCore/QSize>
#include <QtCore/QSharedPointer>

Intersectable * getScene(void)
{
  Material* material(new PhongMaterial(Spectrum(0, 0, .3), Spectrum(1, 1, 1), 64));
  Material* material2(new PhongMaterial(Spectrum(.3, .1, 0), Spectrum(1, 1, 1), 64));

  Intersectable * obj = ObjReader::getMesh("objfiles/teapot.obj", material);
  
  std::list<Intersectable* > scene;
  QMatrix4x4 t;
//   t.scale(0.5);
//   t.translate(0.5, 0, 0);
  scene.push_back(new IntersectableInstance(t, obj));
  scene.push_back(new Plane(QVector4D(0, 1, 0, 0), material));
  
  Intersectable* sphere(new Sphere(QVector3D(0, 1, 2.5), .8, material2));
  scene.push_back(sphere);
  
  return new IntersectableList(scene);
}

Camera getCamera(QSize resolution)
{
  return Camera(QVector3D(0, 1, -5), QVector3D(0, 0.5, 0), QVector3D(0, 1, 0), 1, resolution);
}

std::vector<Light* > getLight(void)
{
  std::vector<Light* > lights;
  lights.push_back(new PointLight(-QVector3D(4, -1, -2), 100 * Spectrum(1, 1, 1)));
  return lights;
}
