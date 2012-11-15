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

#include <QLabel>
#include <QTimer>
#include <QFuture>
#include <QAction>

#include <opencv2/core/core.hpp>

class Win: public QLabel
{
  
  Q_OBJECT
  
public:
  Win(cv::Mat & film, QFuture< void > future): tonemapper(QSize(film.size().width, film.size().height)), film(film), future(future)
  {
    cv::Size size = film.size();
    setPixmap(QPixmap(QSize(size.width, size.height)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.setInterval(100);
    timer.start();

    setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction *saveAct = new QAction("Save image as...", this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveImage()));
    insertAction(0, saveAct);

    saveAct = new QAction("Save image as EXR...", this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveExr()));
    insertAction(0, saveAct);

    setWindowTitle("Rendering...");
    setFixedSize(QSize(film.size().width, film.size().height));
  }
  
private Q_SLOTS:
  void update();
  void saveImage();
  void saveExr();
  
private:
  Tonemapper tonemapper;
  QTimer timer;
  cv::Mat & film;
  QFuture<void> future;
};

#endif
