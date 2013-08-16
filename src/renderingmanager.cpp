#include "renderingmanager.h"

#include "metropolisrenderer.h"
#include "metropolisfltrenderer.h"
#include "perpixelrenderer.h"
#include "energyredistributionrenderer.h"

struct initializer
{
  std::map<std::string, Renderer*> &renderers;
  initializer(std::map<std::string, Renderer*> &renderers) : renderers(renderers) {}

  template< typename R > void operator()(R)
  {
      renderers[R::name] = new R();
  }
};

RenderingManager::RenderingManager()
{
    initializer init(renderers);
    boost::mpl::for_each<Renderers>(init);
}

Renderer* RenderingManager::getRenderer(std::string name)
{
    return renderers[name];
}
