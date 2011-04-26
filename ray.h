#ifndef RAY_H
#define RAY_H

#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>

class Ray
{

  public:
    Ray();
    Ray(QVector3D origin, QVector3D direction);
    Ray(QVector4D origin, QVector4D direction);
    
    QVector3D getOrigin() const;
    QVector3D getDirection() const;
    
    QVector3D evaluate(double u) const;
    
    Ray transform(QMatrix4x4 matrix);

  private:
    void init(QVector4D origin, QVector4D direction);
    
    QVector3D origin, direction;
};

inline QVector3D Ray::getOrigin() const
{
  return origin;
}

inline QVector3D Ray::getDirection() const
{
  return direction;
}

inline Ray::Ray()
{
  init(QVector4D(), QVector4D());
}

inline Ray::Ray(QVector3D origin, QVector3D direction)
{
  init(QVector4D(origin, 1), direction);
}

inline Ray::Ray(QVector4D origin, QVector4D direction)
{
  init(origin, direction);
}

inline void Ray::init(QVector4D origin, QVector4D direction)
{
  this->origin = origin.toVector3DAffine();
  this->direction = direction.toVector3D();
}

inline QVector3D Ray::evaluate(double u) const
{
  return origin + u * direction;
}

inline Ray Ray::transform(QMatrix4x4 matrix)
{
  return Ray(matrix.map(QVector4D(origin, 1)), matrix.map(QVector4D(direction, 0)));
}

#endif // RAY_H
