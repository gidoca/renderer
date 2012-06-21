#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "global.h"
#include "plane.h"
#include "hitrecord.h"

#include <QVector3D>

#include <opencv2/core/core.hpp>

class Triangle : public Intersectable
{
  public:
    Triangle(QVector3D p1, QVector3D p2, QVector3D p3, Material* material) : p1(p1), p2(p2), p3(p3), n1(QVector3D::crossProduct(p1 - p2, p1 - p3)), n2(n1), n3(n1), t1(cv::Point2f(0, 0)), t2(cv::Point2f(0, 1)), t3(cv::Point2f(1, 0)), material(material) {}
    Triangle(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D n1, QVector3D n2, QVector3D n3, Material * material) : p1(p1), p2(p2), p3(p3), n1(n1), n2(n2), n3(n3), t1(cv::Point2f(0, 0)), t2(cv::Point2f(0, 1)), t3(cv::Point2f(1, 0)), material(material) {}
    Triangle(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D n1, QVector3D n2, QVector3D n3, cv::Point2f t1, cv::Point2f t2, cv::Point2f t3, Material * material) : p1(p1), p2(p2), p3(p3), n1(n1), n2(n2), n3(n3), t1(t1), t2(t2), t3(t3), material(material) {}

    HitRecord intersect(Ray ray, float from, float to) const;
    AxisAlignedBox * boundingBox() const;
    
  private:
    QVector3D p1, p2, p3, n1, n2, n3;
    cv::Point2f t1, t2, t3;
    Material* material;
};

#endif // TRIANGLE_H
