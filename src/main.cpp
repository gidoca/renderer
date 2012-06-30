#include <QtCore>
#include <QApplication>
#include <QTime>

#include <list>
#include <vector>
#include <iostream>

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
#include "symmetricfilter.h"

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

void render(Renderer * renderer, cv::Mat film, Scene scene, variables_map vm)
{  
  QTime time;
  time.start();
  renderer->render(scene, film, vm);
  if(vm.count("save-exr")) {
      imwrite(vm["save-exr"].as<string>(), film);
  }
//  if(vm.count("save-img")) film.saveImg(vm["save-img"].as<string>());
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
      ("scene,s", value<string>(), "the scene description file (mandatory)");
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

  Renderer * renderer = getRendererByName(vm["renderer"].as<string>());
  if(renderer == 0)
  {
    cerr << "Unknown renderer: " << vm["renderer"].as<string>() << endl;
    return -1;
  }

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
  
  cv::Mat film(scene.camera.getResolution().height(), scene.camera.getResolution().width(), CV_32FC3);
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


/*
int main(void)
{
    cv::Mat imgorig = cv::imread("ramp.png");
    cv::Mat guideorig = cv::imread("ramp_guide.png");
    cv::Mat pix_var = 0.0025 * cv::Mat::ones(imgorig.size(), CV_32FC1);

    cv::Mat imggray, guidegray;
    cv::cvtColor(imgorig, imggray, CV_RGB2GRAY);
    cv::cvtColor(guideorig, guidegray, CV_RGB2GRAY);

    cv::Mat img, guide;
    imggray.convertTo(img, CV_32FC1, 1./255);
    guidegray.convertTo(guide, CV_32FC1, 1./255);

    SymmetricFilter f;
    cv::Mat out = f.filter(img, guide, pix_var);

    cv::imshow("filtered", out);
    cv::waitKey();
}
*/
