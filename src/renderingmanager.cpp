#include "renderingmanager.h"

#include "metropolisrenderer.h"
#include "metropolisfltrenderer.h"
#include "perpixelrenderer.h"
#include "energyredistributionrenderer.h"

struct initializer
{
  std::map<std::string, Renderer*> &renderers;
  cv::Mat *film;
  const boost::program_options::variables_map vm;
  initializer(std::map<std::string, Renderer*> &renderers, cv::Mat *film, const boost::program_options::variables_map vm) : renderers(renderers), film(film), vm(vm) {}

  template< typename R > void operator()(R)
  {
      renderers[R::name] = new R();
      renderers[R::name]->setOutput(film);
      renderers[R::name]->setOptions(vm);
  }
};

RenderingManager::RenderingManager(cv::Mat *film, const boost::program_options::variables_map vm) : currentRenderer(nullptr)
{
    initializer init(renderers, film, vm);
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

