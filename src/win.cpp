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
#include <QKeyEvent>

#include <opencv2/highgui/highgui.hpp>

using namespace cv;
#include <iostream>
void Win::update()
{
    QImage image = tonemapper.tonemap(film);
    setPixmap(QPixmap::fromImage(image));
    repaint();
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
//    timer.start();

    setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction *saveAct = new QAction("Save image as...", this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveImage()));
    insertAction(0, saveAct);

    saveAct = new QAction("Save image as EXR...", this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveExr()));
    insertAction(0, saveAct);

    setWindowTitle("Idle");
    setFixedSize(QSize(film.size().width, film.size().height));
}

inline void Win::move(short sign, bool strafe)
{
    QVector3D dir = scene.camera.getLookAt() - scene.camera.getCOP();
    if(strafe) dir = QVector3D::crossProduct(dir, scene.camera.getUp());
    dir.normalize();
    dir *= sign * stepSize;
    scene.camera.setCOP(scene.camera.getCOP() + dir);
    scene.camera.setLookAt(scene.camera.getLookAt() + dir);
}

inline void Win::rotate(short sign, bool horizontal)
{
    QVector3D dir = scene.camera.getLookAt() - scene.camera.getCOP();
    QVector3D axis;
    if(horizontal)
    {
        axis = QVector3D::crossProduct(dir, QVector3D::crossProduct(scene.camera.getUp(), dir));
    }
    else
    {
        axis = QVector3D::crossProduct(scene.camera.getUp(), dir);
    }
    QMatrix4x4 rot;
    rot.rotate(sign * 3, axis);
    dir = rot.map(dir);
    scene.camera.setLookAt(scene.camera.getCOP() + dir);
    scene.camera.setUp(rot.map(scene.camera.getUp()));
}

void Win::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_F5:
        break;

    case Qt::Key_R:
        scene.camera = originalCamera;
        stepSize = 1;
        break;

    case Qt::Key_Q:
        stepSize /= 1.5;
        return;
    case Qt::Key_E:
        stepSize *= 1.5;
        return;

    case Qt::Key_S:
        move(-1, false);
        break;
    case Qt::Key_W:
        move(1, false);
        break;
    case Qt::Key_A:
        move(-1, true);
        break;
    case Qt::Key_D:
        move(1, true);
        break;

    case Qt::Key_Down:
        rotate(1, false);
        break;
    case Qt::Key_Up:
        rotate(-1, false);
        break;
    case Qt::Key_Left:
        rotate(1, true);
        break;
    case Qt::Key_Right:
        rotate(-1, true);
        break;

    default:
        QWidget::keyPressEvent(event);
        return;
    }

    std::cerr << scene.camera << std::endl;

    Q_EMIT rerender(scene);
}

void Win::starting()
{
    setWindowTitle("Rendering...");
    timer.start();
}

void Win::complete()
{
    timer.stop();
    update();
    setWindowTitle("Rendering complete.");
}
