#include <QVector3D>

#include "../camera.h"
#include "../intersectablelist.h"
#include "../plane.h"
#include "../quad.h"
#include "../sphere.h"
#include "../arealight.h"
#include "../phongmaterial.h"

Camera getCamera(QSize resolution)
{
  // Make camera and film
  QVector3D eye(0.f,1.f,0.f);
  QVector3D lookAt(0.f,1.f,1.f);
  QVector3D up(0.f,1.f,0.f);
  float fov = M_PI / 3;
  return Camera(eye, lookAt, up, fov, resolution);
}

Intersectable * getScene(void)
{
  std::list<Intersectable *> intersectables;

  intersectables.push_back(new Plane(QVector4D(0, 1, 0, 0), new PhongMaterial(Spectrum(.4, .2, .1), Spectrum(), 1)));
  intersectables.push_back(new Plane(QVector4D(0, -1, 0, 2), new PhongMaterial(Spectrum(.4, .5, .5), Spectrum(), 1)));

  intersectables.push_back(new Quad(QVector3D(1, 0, 5), QVector3D(-1, 0, 5), QVector3D(-1, 2, 5), QVector3D(1, 2, 5), new PhongMaterial(Spectrum(.4, .5, .5), Spectrum(), 1)));
  intersectables.push_back(new Quad(QVector3D(-1, 0, -1), QVector3D(1, 0, -1), QVector3D(1, 2, -1), QVector3D(-1, 2, -1), new PhongMaterial(Spectrum(.4, .5, .5), Spectrum(), 1)));
  intersectables.push_back(new Quad(QVector3D(1, 0, 5), QVector3D(1, 2, 5), QVector3D(1, 2, -1), QVector3D(1, 0, -1), new PhongMaterial(Spectrum(.4, .5, .5), Spectrum(), 1)));
  intersectables.push_back(new Quad(QVector3D(-1, 2, 3), QVector3D(-1, 0, 3), QVector3D(-1, 0, -1), QVector3D(-1, 2, -1), new PhongMaterial(Spectrum(.4, .5, .5), Spectrum(), 1)));
  intersectables.push_back(new Quad(QVector3D(-1, 1.8, 4), QVector3D(-1, 0, 4), QVector3D(-1.3, 0, 3), QVector3D(-1.3, 1.8, 3), new PhongMaterial(Spectrum(.4, .2, 0), Spectrum(), 1)));
  intersectables.push_back(new Quad(QVector3D(-1, 2, 4), QVector3D(-1, 1.8, 4), QVector3D(-1, 1.8, 3), QVector3D(-1, 2, 3), new PhongMaterial(Spectrum(.4, .5, .5), Spectrum(), 1)));
  intersectables.push_back(new Quad(QVector3D(-1, 2, 5), QVector3D(-1, 0, 5), QVector3D(-1, 0, 4), QVector3D(-1, 2, 4), new PhongMaterial(Spectrum(.4, .5, .5), Spectrum(), 1)));

  return new IntersectableList(intersectables);
}

std::vector<const Light* > getLight(void)
{
  std::vector<const Light* > lights;
  lights.push_back(new AreaLight(QVector3D(-2., 2., 5.), QVector3D(.5, 0, 0), QVector3D(0, 0, .5), 32000 * Spectrum(1, 1, 1)));
  return lights;
}
