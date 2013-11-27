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

#include "global.h"

#include <QtCore>
#include <QApplication>
#include <QTime>

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <functional>

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
#include "scenedumper.h"
#include "renderingmanager.h"

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

variables_map vm;


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
      ("save-img,i", value<string>(), "write the result to the specified LDR image file")
      ("dump-bvh,b", value<string>(), "write the bvh tree to the specified file");
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
  boost::mpl::for_each<Renderers>(option_adder(&command_line_options));

  try
  {
    store(parse_command_line(argc, argv, command_line_options), vm);
  }
  catch(boost::program_options::error& e)
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

  if(vm.count("verbose")) cerr << "Loading scene..." << endl;

  SceneGrammar parser;
  if(!vm.count("scene"))
  {
    cerr << "Please specify the scene description file by using the -s option" << endl;
    return -1;
  }
  if(!parser.parse(vm["scene"].as<string>()))
  {
    cerr << "Failed to parse scene" << endl;
    return -1;
  }

  std::vector<ast_assignment> ast = parser.getAst();
  resolveVars(ast);

  bool hasGui = vm.count("gui") || (!vm.count("save-exr") && !vm.count("save-img") && !vm.count("dump-bvh"));
  bool saveToFile = vm.count("save-img") || vm.count("save-exr");

  if(vm.count("dump-bvh"))
  {
      ofstream bvhfile(vm["dump-bvh"].as<string>());
      SceneDumper d(bvhfile);
      std::vector<ast_assignment> bvhAst = createBVH(ast);
      if(vm.count("verbose")) cerr << "Writing BVH tree to " << vm["dump-bvh"].as<string>() << endl;
      d.dump(bvhAst);
      bvhfile.close();
      if(!hasGui && !saveToFile)
      {
          if(vm.count("verbose")) cerr << "Dumping BVH complete, " << time.elapsed() / 1000 << "s elapsed." << endl;
          return 0;
      }
  }

  Scene scene = buildScene(ast);

  if(vm.count("verbose")) cerr << "Scene loaded, " << time.elapsed() / 1000 << "s elapsed." << endl;

  Tonemapper tm(scene.camera.getResolution(), vm["gamma"].as<float>());

  cv::Mat * film = new cv::Mat(scene.camera.getResolution().height(), scene.camera.getResolution().width(), CV_32FC3);

  RenderingManager manager(film);
  try
  {
      manager.setCurrentRenderer(vm["renderer"].as<string>());
  }
  catch(std::string& error)
  {
      cerr << "Error: " << error << endl;
      return -1;
  }

  if(vm.count("verbose"))
  {
      QObject::connect(&manager, &RenderingManager::finishedRendering, [=](){
          cerr << "Rendering complete, " << time.elapsed() / 1000 << "s elapsed." << endl;
      });
  }
  
  Win l(*film, scene, tm);

  if(hasGui)
  {
    QObject::connect(&manager, &RenderingManager::finishedRendering, &l, &Win::complete, Qt::QueuedConnection);
    QObject::connect(&manager, &RenderingManager::startingRendering, &l, &Win::starting, Qt::QueuedConnection);
    QObject::connect(&l, &Win::rerender, &manager, &RenderingManager::startRendering);
    QObject::connect(&l, &Win::changeRenderer, [&](std::string name) mutable {
        manager.setCurrentRenderer(name);
        manager.startRendering(scene);
    });
    l.show();
  }

  if(vm.count("save-exr"))
  {
      QObject::connect(&manager, &RenderingManager::finishedRendering, [=](){
          imwrite(vm["save-exr"].as<string>(), *film);
      });
  }

  if(vm.count("save-img"))
  {
      QObject::connect(&manager, &RenderingManager::finishedRendering, [=]() mutable {
          QImage img = tm.tonemap(*film);
          img.save(QString::fromStdString(vm["save-img"].as<string>()));
      });
  }

  if(saveToFile && !hasGui)
  {
      QObject::connect(&manager, &RenderingManager::finishedRendering, &QApplication::quit);
  }

  manager.startRendering(scene);

  return app.exec();
}
