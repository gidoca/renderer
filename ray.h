#ifndef RAY_H
#define RAY_H

#include <QVector3D>

class Ray
{

  public:
    Ray(QVector3D origin, QVector3D direction);
    
    const QVector3D getOrigin();
    const QVector3D getDirection();

  private:
    QVector3D origin, direction;
};

#endif // RAY_H
