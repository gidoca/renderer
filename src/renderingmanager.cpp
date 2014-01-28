#include "renderingmanager.h"

#include "metropolisrenderer.h"
#include "metropolisfltrenderer.h"
#include "perpixelrenderer.h"
#include "energyredistributionrenderer.h"

struct initializer
{
  std::map<std::string, Renderer*> &renderers;
  cv::Mat *film;
  initializer(std::map<std::string, Renderer*> &renderers, cv::Mat *film) : renderers(renderers), film(film) {}

  template< typename R > void operator()(R)
  {
      renderers[R::name] = new R();
      renderers[R::name]->setOutput(film);
  }
};

RenderingManager::RenderingManager(cv::Mat *film) : currentRenderer(nullptr)
{
    initializer init(renderers, film);
    boost::mpl::for_each<Renderers>(init);
}

void RenderingManager::setCurrentRenderer(std::string name)
{
    if(currentRenderer != nullptr)
    {
        currentRenderer->stopRendering();
        disconnect(currentRenderer);
    }

    currentRenderer = renderers[name];
    if(currentRenderer == nullptr)
    {
        throw "Renderer named " + name + " does not exist";
    }
    connect(currentRenderer, &Renderer::finishedRendering, [=](){
        Q_EMIT finishedRendering();
    });
}

void RenderingManager::startRendering(Scene scene)
{
    assert(currentRenderer != nullptr);
    currentRenderer->prepareRenderingScene(scene);
    Q_EMIT startingRendering();
    currentRenderer->start();
}

void RenderingManager::stopRendering()
{
    if(currentRenderer != nullptr)
    {
        currentRenderer->stopRendering();
    }
}
