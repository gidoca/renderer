#include "win.h"
#include "win.moc"

#include <QImage>
#include <QFileDialog>
#include <QImageWriter>

void Win::update()
{
    QImage image = tonemapper.tonemap(film);
    setPixmap(QPixmap::fromImage(image));
    repaint();
    if(future.isFinished())
    {
      timer.stop();
      setWindowTitle("Rendering complete.");
    }
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

  QString file = QFileDialog::getSaveFileName(this, "Save image as",  QString(), formatString);
  if(file.isNull()) return;
  QImage image = tonemapper.tonemap(film);
  image.save(file);
}
