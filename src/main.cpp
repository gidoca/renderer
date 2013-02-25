/**
 * Copyright (C) 2012 Gian Calgeer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <QtCore>
#include <QApplication>
#include <QTime>

#include <list>
#include <vector>
#include <iostream>
#include <functional>

#include <boost/program_options.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "perpixelrenderer.h"
#include "unidipathtracingintegrator.h"
#include "metropolisrenderer.h"
#include "metropolisfltrenderer.h"
#include "energyredistributionrenderer.h"
#include "scene.h"
#include "tonemapper.h"
#include "win.h"
#include "sceneparser.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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

int main(int argc, char **argv) {
  QTime time;
  time.start();

  QApplication app(argc, argv);
  options_description command_line_options;

  options_description general("General options");
  general.add_options()
      ("help,h", "display the help")
      ("verbose,v", "be verbose about progress, etc. ")
      ("gui,u", "display the result in a window")
      ("save-exr,e", value<string>(), "write the result to the specified EXR file")
      ("save-img,i", value<string>(), "write the result to the specified LDR image file");
  command_line_options.add(general); 
	
  options_description image("Image options");
	image.add_options()
      ("renderer,r", value<string>()->default_value("pathtracing"), "the rendering algorithm to be used (either pathtracing (the default), energyredist, metropolisflt, or metropolis)")
      ("fixed-seed,d", "use a fixed seed for the RNG to make the resulting image deterministic")
      ("seed", value<unsigned long>()->default_value(0), "the random generator seed to use when the --fixed-seed option is set")
      ("scene,s", value<string>(), "the scene description file (mandatory)")
      ("gamma,g", value<float>()->default_value(2.2f, "2.2"), "the gamma correction to apply to the display and to LDR image file output");
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

  if(vm.count("verbose")) cout << "Loading scene..." << endl;

  SceneGrammar parser;
  if(!vm.count("scene"))
  {
    cerr << "Please specify the scene description file by using the -s option\n";
    return -1;
  }
  if(!parser.parse(vm["scene"].as<string>()))
  {
    cerr << "Failed to parse scene\n";
    return -1;
  }

  Scene scene = buildScene(parser.getAst());

  if(vm.count("verbose")) cout << "Scene loaded, " << time.elapsed() / 1000 << "s elapsed." << endl;

  Tonemapper tm(scene.camera.getResolution(), vm["gamma"].as<float>());

  cv::Mat * film = new cv::Mat(scene.camera.getResolution().height(), scene.camera.getResolution().width(), CV_32FC3);

  Renderer * renderer = getRendererByName(vm["renderer"].as<string>());
  if(renderer == 0)
  {
    cerr << "Unknown renderer: " << vm["renderer"].as<string>() << endl;
    return -1;
  }

  renderer->setOutput(film);
  renderer->setOptions(vm);
  renderer->startRendering(scene);
  
  if(vm.count("gui") || (!vm.count("save-exr") && !vm.count("save-img")))
  {
    Win l(*film, scene, tm);
    QObject::connect(renderer, SIGNAL(finishedRendering()), &l, SLOT(complete()));
    QObject::connect(renderer, SIGNAL(startingRendering()), &l, SLOT(starting()));
    QObject::connect(&l, SIGNAL(rerender(Scene)), renderer, SLOT(startRendering(Scene)));
    l.show();
  
    return app.exec();
  }
  else
  {
    renderer->wait();
    if(vm.count("save-exr"))
    {
        imwrite(vm["save-exr"].as<string>(), *film);
    }
    if(vm.count("save-img"))
    {
        QImage img = tm.tonemap(*film);
        img.save(QString(vm["save-img"].as<string>().c_str()));
    }
    return 0;
  }
}
