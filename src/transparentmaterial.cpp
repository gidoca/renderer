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

#include "mathhelper.h"

TransparentMaterial::TransparentMaterial(float refractionCoeff) : refractionCoeff(refractionCoeff)
{
}

QVector3D TransparentMaterial::outDirection(QVector3D inDirection, QVector3D normal, Sample s) const
{
    inDirection.normalize();
    normal.normalize();
    float cosIncomingAngle = QVector3D::dotProduct(normal, -inDirection);
    float coefficientRatio = 1 / refractionCoeff;
    float cosOutgoingAngle = sqrt(1 - coefficientRatio * coefficientRatio * (1 - cosIncomingAngle * cosIncomingAngle));
    float rPerp = (cosIncomingAngle - cosOutgoingAngle) / (cosIncomingAngle + cosOutgoingAngle);
    float reflectance = (1 + coefficientRatio * coefficientRatio) / 2 * rPerp * rPerp;
    if(abs(cosOutgoingAngle) >= 1 || s.getSample().x() < reflectance)
    {
        return inDirection + 2 * cosIncomingAngle * normal;
    }
    else
    {
        return coefficientRatio * inDirection + (coefficientRatio * cosIncomingAngle - signum(cosIncomingAngle) * cosOutgoingAngle) * normal;
    }
}
