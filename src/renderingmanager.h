#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#include "global.h"

#include <map>
#include <string>

#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>

typedef boost::mpl::list<MetropolisRenderer, MetropolisFltRenderer, PerPixelRenderer, EnergyRedistributionRenderer> Renderers;

class RenderingManager
{
public:
    RenderingManager();

    Renderer* getRenderer(std::string name);

private:
    std::map<std::string, Renderer*> renderers;
};

#endif // RENDERINGMANAGER_H
