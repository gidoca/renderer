#ifndef RAY_H
#define RAY_H

#include <QVector3D>
#include <QMatrix4x4>

class Ray
{

  public:
    Ray(QVector3D origin, QVector3D direction);
    Ray(QVector4D origin, QVector4D direction);
    
    QVector4D getOrigin() const;
    QVector4D getDirection() const;
    
    QVector3D evaluate(double u) const;
    
    Ray transform(QMatrix4x4 matrix);

  private:
    void init(QVector4D origin, QVector4D direction);
    
    QVector4D origin, direction;
};

#endif // RAY_H
