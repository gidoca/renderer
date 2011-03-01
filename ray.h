#ifndef RAY_H
#define RAY_H

#include <QVector3D>
#include <QMatrix4x4>

class Ray
{

  public:
    Ray(QVector3D origin, QVector3D direction);
    
    QVector3D getOrigin() const;
    QVector3D getDirection() const;
    
    QVector3D evaluate(double u) const;
    
    Ray transform(QMatrix4x4 matrix);

  private:
    QVector3D origin, direction;
};

#endif // RAY_H
