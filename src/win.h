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
