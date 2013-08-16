#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#include "global.h"

#include <QObject>

#include <map>
#include <string>

#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/program_options.hpp>

#include <opencv2/core/core.hpp>

typedef boost::mpl::list<MetropolisRenderer, MetropolisFltRenderer, PerPixelRenderer, EnergyRedistributionRenderer> Renderers;

class RenderingManager : public QObject
{
    Q_OBJECT

public:
    RenderingManager(cv::Mat * film, const boost::program_options::variables_map vm);

    void setCurrentRenderer(std::string name);

public Q_SLOTS:
  void startRendering(Scene scene);

Q_SIGNALS:
  void startingRendering();
  void finishedRendering();

private:
    std::map<std::string, Renderer*> renderers;
    Renderer* currentRenderer;
};

#endif // RENDERINGMANAGER_H
