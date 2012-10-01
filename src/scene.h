#ifndef SCENE_H
#define SCENE_H

#include "global.h"
#include "camera.h"

#include <vector>
#include <map>

class Scene
{
public:
  Scene(Camera camera): camera(camera) {}

  const Intersectable * object;
  Camera camera;
  std::vector<const Light*> light;
};

#endif // SCENE_H
