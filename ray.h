#ifndef RAY_H
#define RAY_H

#include <QVector3D>

class Ray
{

  public:
    Ray(QVector3D origin, QVector3D direction);
    
    QVector3D getOrigin() const;
    QVector3D getDirection() const;
    
    QVector3D evaluate(double u) const;

  private:
    QVector3D origin, direction;
};

#endif // RAY_H
