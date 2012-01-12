#ifndef PERPIXELRENDERER_H
#define PERPIXELRENDERER_H

#include <QImage>
#include <QSize>
#include <QSharedPointer>

#include <list>

class Intersectable;
class Light;
class Camera;

class PerPixelRenderer
{
public:
	PerPixelRenderer(QSize size);
	
	QImage render(const Intersectable & scene, const Camera & camera, std::list<QSharedPointer<Light> > lights);
	
private:
	QImage image;
};

#endif // PERPIXELRENDERER_H
