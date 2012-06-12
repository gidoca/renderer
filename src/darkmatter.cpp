#include "darkmatter.h"

DarkMatter::DarkMatter()
{

}

cv::Vec3f DarkMatter::shade(const HitRecord&, QVector3D) const
{
  return cv::Vec3f(0, 0, 0);
}

bool DarkMatter::isMirror() const
{
  return false;
}

DarkMatter* DarkMatter::instance(new DarkMatter());
