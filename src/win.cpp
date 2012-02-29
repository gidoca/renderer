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

  QString filename = QFileDialog::getSaveFileName(this, "Save image as",  QString(), formatString);
  if(filename.isNull()) return;
  film.saveImg(filename.toStdString());
}

void Win::saveExr()
{
  QString formatString = "OpenEXR (*.exr)";

  QString filename = QFileDialog::getSaveFileName(this, "Save image as",  QString(), formatString);
  if(filename.isNull()) return;
  film.saveExr(filename.toStdString());
}
