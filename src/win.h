#ifndef WIN_H
#define WIN_H

#include <QLabel>
#include <QTimer>
#include <QFuture>
#include <QAction>

#include "film.h"
#include "tonemapper.h"

class Win: public QLabel
{
  
  Q_OBJECT
  
public:
  Win(Film film, QFuture< void > future): film(film), tonemapper(film.getSize()), future(future)
  {
    setPixmap(QPixmap(film.getSize()));
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
  }
  
private slots:
  void update();
  void saveImage();
  void saveExr();
  
private:
  QTimer timer;
  Film film;
  Tonemapper tonemapper;
  QFuture<void> future;
};

#endif
