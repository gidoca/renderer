/**
 * Copyright (C) 2012 Gian Calgeer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "transparentmaterial.h"

#include <iostream>
#include <QMatrix4x4>

#include "mathhelper.h"

TransparentMaterial::TransparentMaterial(float refractionCoeff) : refractionCoeff(refractionCoeff)
{
}

QVector3D TransparentMaterial::outDirection(QVector3D inDirection, QVector3D normal, Sample s) const
{
    inDirection.normalize();
    normal.normalize();
    float coefficientRatio;
    if(QVector3D::dotProduct(-normal, inDirection) < 0.)
    {
        normal *= -1;
        coefficientRatio = refractionCoeff;
    }
    else
    {
        coefficientRatio = 1 / refractionCoeff;
    }
    float cosIncomingAngle = QVector3D::dotProduct(-normal, inDirection);
    float sinOutgoingAngle = coefficientRatio * sqrt(std::max(0., 1. - cosIncomingAngle * cosIncomingAngle));
    //float sinIncomingAngle = sqrt(1 - cosIncomingAngle * cosIncomingAngle);
    //std::cerr << sinOutgoingAngle / sinIncomingAngle << std::endl;
    float cosOutgoingAngle = sqrt(std::max(0., 1 - coefficientRatio * coefficientRatio * std::max(0., 1. - cosIncomingAngle * cosIncomingAngle)));
    float rPerp = (cosIncomingAngle - coefficientRatio * cosOutgoingAngle) / (cosIncomingAngle + coefficientRatio * cosOutgoingAngle);
    float rPara = (coefficientRatio * cosIncomingAngle - cosOutgoingAngle) / (coefficientRatio * cosIncomingAngle + cosOutgoingAngle);
    float reflectance = 1 / 2. * (rPerp * rPerp + rPara * rPara);
    if(sinOutgoingAngle > 1 || s.getSample().x() < reflectance)
    {
        QVector3D v = inDirection + 2 * cosIncomingAngle * normal;
        return v;
    }
    else
    {
        QVector3D v = coefficientRatio * inDirection + (coefficientRatio * cosIncomingAngle - signum(cosIncomingAngle) * cosOutgoingAngle) * normal;
        //std::cerr << sqrt(1 - QVector3D::dotProduct(v, -normal) * QVector3D::dotProduct(v, -normal)) / sqrt(1 - cosIncomingAngle * cosIncomingAngle) << std::endl;
        /*QMatrix4x4 m;
        m.setColumn(0, v);
        m.setColumn(1, normal);
        m.setColumn(2, inDirection);
        std::cerr << m.determinant() << std::endl;*/
        return v;
    }
}
