#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <QSharedPointer>

#include "film.h"
#include "path.h"
#include "spectrum.h"

#define MAX_DEPTH 4

class Intersectable;
class Light;
class Camera;
class Ray;

class Renderer
{
public:
  virtual Film render(const Intersectable & scene, const Camera & camera, std::vector<QSharedPointer<Light> > lights) = 0;
  
  static Path createPath(const Ray& primaryRay, const Intersectable& scene, Spectrum initialAlpha = Spectrum(1, 1, 1));
};

#endif // RENDERER_H
