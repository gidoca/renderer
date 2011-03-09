#include "objreader.h"
#include "pointlight.h"
#include "camera.h"
#include "diffusematerial.h"

#include <list>
#include <QSize>
#include <QSharedPointer>

Intersectable * getScene(void)
{
  QSharedPointer<Material> material(new DiffuseMaterial(Spectrum(0, 0, 1), Spectrum(0, 0, 1), 64));

  return ObjReader::getMesh("objfiles/teapot.obj", material);
}

Camera getCamera(QSize resolution)
{
  return Camera(QVector3D(0, 1, 5), QVector3D(0, 0.5, 0), QVector3D(0, 1, 0), 1, resolution);
}

Light * getLight(void)
{
  return new PointLight(QVector3D(0, 1, 2), Spectrum(1, 1, 1));
}