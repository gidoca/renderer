#ifndef WIN_H
#define WIN_H

#include <QLabel>
#include <QTimer>

#include "film.h"
#include "tonemapper.h"

class Win: public QLabel
{
  
  Q_OBJECT
  
public:
  Win(Film film): film(film), tonemapper(film.getSize())
  {
    setPixmap(QPixmap(film.getSize()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.setInterval(100);
    timer.start();
  }
  
private slots:
  void update();
  
private:
  QTimer timer;
  Film film;
  Tonemapper tonemapper;
};

#endif