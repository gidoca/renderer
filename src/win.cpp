#include "win.h"
#include "win.moc"

#include <QImage>
#include <QFileDialog>
#include <QImageWriter>

#include <ImfRgbaFile.h>

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

void Win::saveExr()
{
  QString formatString = "OpenEXR (*.exr)";

  QString filename = QFileDialog::getSaveFileName(this, "Save image as",  QString(), formatString);
  if(filename.isNull()) return;
  Imf::Rgba *rgba = film.toExrRgba();
  Imf::RgbaOutputFile file(filename.toUtf8(), film.getSize().width(), film.getSize().height(), Imf::WRITE_RGBA);
  file.setFrameBuffer(rgba, 1, film.getSize().width());
  file.writePixels(film.getSize().height());
}
