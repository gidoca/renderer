#include "win.h"
#include "win.moc"

#include <QImage>

void Win::update()
{
    QImage image = tonemapper.tonemap(film);
    setPixmap(QPixmap::fromImage(image));
    repaint();
    if(future.isFinished()) timer.stop();
}
