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
#ifndef WIN_H
#define WIN_H

#include "tonemapper.h"

#include "scene.h"

#include <QLabel>
#include <QTimer>
#include <QFuture>
#include <QAction>

#include <opencv2/core/core.hpp>

class Win: public QLabel
{
  
  Q_OBJECT
  
public:
  Win(cv::Mat & film, Scene scene) : tonemapper(QSize(film.size().width, film.size().height)), film(film), scene(scene), stepSize(1), originalCamera(scene.camera)
  {
      init();
  }

  Win(cv::Mat &film, Scene scene, Tonemapper tonemapper) : tonemapper(tonemapper), film(film), scene(scene), stepSize(1), originalCamera(scene.camera)
  {
      init();
  }

public Q_SLOTS:
  void starting();
  void complete();
  
private Q_SLOTS:
  void update();
  void saveImage();
  void saveExr();

Q_SIGNALS:
  void rerender(Scene scene);
  void changeRenderer(std::string name);

protected:
  void keyReleaseEvent(QKeyEvent * event);
  void wheelEvent(QWheelEvent * event);

private:
  void init();

  void move(short sign, bool strafe);
  void rotate(short sign, bool horizontal);
  
  Tonemapper tonemapper;
  QTimer timer;
  cv::Mat & film;
  Scene scene;

  float stepSize;
  Camera originalCamera;
};

#endif
