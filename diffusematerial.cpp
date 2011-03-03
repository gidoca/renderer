#include "diffusematerial.h"

DiffuseMaterial::DiffuseMaterial(Spectrum color) : color(color)
{

}

Spectrum DiffuseMaterial::shade(QVector3D location, QVector3D normal) const
{
  return color;
}
