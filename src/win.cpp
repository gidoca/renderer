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
#include "win.h"

#include <QImage>
#include <QFileDialog>
#include <QImageWriter>

#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void Win::update()
{
    QImage image = tonemapper.tonemap(film);
    setPixmap(QPixmap::fromImage(image));
    repaint();
    /*if(future.isFinished())
    {
      timer.stop();
      setWindowTitle("Rendering complete.");
    }*/
}

void Win::saveImage()
{
  QList<QByteArray> formatsByte = QImageWriter::supportedImageFormats();
  QStringList formats;

  for (int i=0; i<formats.count(); i++)
  {
    QString current = formatsByte[i];
    current.prepend("*.");
    formats.append(current);
  }

  QString formatString = "Image File (" + formats.join(" ").toLower() + ")";

  QString filename = QFileDialog::getSaveFileName(this, "Save image as",  QString(), formatString);
  if(filename.isNull()) return;

  Tonemapper tonemapper(QSize(film.size().width, film.size().height));
  QImage image = tonemapper.tonemap(film);
  image.save(filename);
}

void Win::saveExr()
{
  QString formatString = "OpenEXR (*.exr)";

  QString filename = QFileDialog::getSaveFileName(this, "Save image as",  QString(), formatString);
  if(!filename.endsWith(".exr")) filename += ".exr";
  if(filename.isNull()) return;
  imwrite(filename.toStdString(), film);
}

void Win::init()
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
