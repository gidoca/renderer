#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <QSharedPointer>

#include "film.h"

class Intersectable;
class Light;
class Camera;

class Renderer
{
public:
  virtual Film render(const Intersectable & scene, const Camera & camera, std::vector<QSharedPointer<Light> > lights) = 0;
};

#endif // RENDERER_H