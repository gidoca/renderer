#include <QtCore>
#include <QApplication>
#include <QTime>

#include <list>
#include <vector>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>

#include "scenes/cornellscene.h"
#include "perpixelrenderer.h"
#include "unidipathtracingintegrator.h"
#include "metropolisrenderer.h"
#include "energyredistributionrenderer.h"
#include "film.h"
#include "scene.h"
#include "tonemapper.h"
#include "win.h"
#include "sceneparser.h"

using namespace std;
using namespace boost::program_options;

struct option_adder
{
  options_description * desc;
  option_adder(options_description * desc): desc(desc) {}

  template< typename R > void operator()(R)
  {
    desc->add(R::options());
  }
};

struct initializer
{
  Renderer **renderer;
  variables_map vm;
  initializer(Renderer **renderer, variables_map vm) : renderer(renderer), vm(vm) {}

  template< typename R > void operator()(R)
  {
    if(vm["renderer"].as<string>() == R::name)
    {
      *renderer = new R();
    }
  }
};

typedef boost::mpl::list<MetropolisRenderer, PerPixelRenderer, EnergyRedistributionRenderer> renderers;

void render(Renderer * renderer, Film film, Scene scene, variables_map vm)
{  
  QTime time;
  time.start();
  renderer->render(scene, film, vm);
  if(vm.count("save-exr")) film.saveExr(vm["save-exr"].as<string>());
  if(vm.count("save-img")) film.saveImg(vm["save-img"].as<string>());
  delete renderer;
  if(vm.count("verbose"))
  {
    std::cout << "Rendering complete, " << time.elapsed() / 1000 << "s elapsed\n";
    std::cout.flush();
  }
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  options_description command_line_options;

  options_description general("General options");
  general.add_options()
      ("help,h", "display the help")
      ("verbose,v", "be verbose about progress, etc. ")
      ("gui,g", "display the result in a window")
      ("save-exr,e", value<string>(), "write the result to the specified EXR file")
      ("save-img,i", value<string>(), "write the result to the specified LDR image file");
  command_line_options.add(general); 
	
  options_description image("Image options");
	image.add_options()
      ("renderer,r", value<string>()->default_value("pathtracing"), "the rendering algorithm to be used (either pathtracing or metropolis)")
      ("width,x", value<int>()->default_value(250), "the width of the output image")
      ("height,y", value<int>()->default_value(250), "the height of the output image");
  command_line_options.add(image);

  //Yay, metaprogramming - because we can!
  boost::mpl::for_each<renderers>(option_adder(&command_line_options));

  variables_map vm;
  try
  {
    store(parse_command_line(argc, argv, command_line_options), vm);
  }
  catch(boost::program_options::error e)
  {
    std::cerr << e.what() << endl;
    return -1;
  }

  notify(vm);

  if(vm.count("help"))
  {
    cerr << command_line_options << endl;
    return 0;
  }

  Renderer * renderer = 0;
  boost::mpl::for_each<renderers>(initializer(&renderer, vm));
  if(renderer == 0)
  {
    cerr << "Unknown renderer: " << vm["renderer"].as<string>() << endl;
    return -1;
  }

  QSize resolution(vm["width"].as<int>(), vm["height"].as<int>());

  SceneGrammar parser;
  if(!parser.parse("list{ sphere([185 83.5 169], 120, diffuse([1 0 0])) instance([[1 0 0][0 1 0][0 0 1]], quad([338 136 351], [338 196 351], [398 196 351], [398 136 351], diffuse([1 0 1]))) }"))
  {
    cerr << "Failed to parse scene\n";
    return -1;
  }

  const Intersectable * object = buildScene(parser.getAst());
  const vector<const Light*> light = getLight();
  const Camera camera = getCamera(resolution);
  
  Film film(resolution);
  Scene scene(camera);
  scene.object = object;
  scene.light = light;
  QFuture< void > future = QtConcurrent::run(render, renderer, film, scene, vm);
  
  if(vm.count("gui") || (!vm.count("save-exr") && !vm.count("save-img")))
  {
    Win l(film, future);
    l.show();
  
    return app.exec();
  }
  else
  {
    future.waitForFinished();
    return 0;
  }
}

