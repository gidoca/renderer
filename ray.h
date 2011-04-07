#ifndef RAY_H
#define RAY_H

#include <QVector3D>
#include <QMatrix4x4>

class Ray
{

  public:
    Ray();
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

inline QVector4D Ray::getOrigin() const
{
  return origin;
}

inline QVector4D Ray::getDirection() const
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
  this->origin = origin;
  this->direction = direction;
}

inline QVector3D Ray::evaluate(double u) const
{
  return origin.toVector3DAffine() + u * direction.toVector3D();
}

inline Ray Ray::transform(QMatrix4x4 matrix)
{
  return Ray(matrix.map(origin), matrix.map(direction));
}

#endif // RAY_H
