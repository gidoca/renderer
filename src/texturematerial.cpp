#include "texturematerial.h"

#include "hitrecord.h"

#include <opencv2/highgui/highgui.hpp>

#include <cmath>
#include <iostream>

using namespace cv;

inline float clamp(float x)
{
    return x <= 0 ? 0 : (x >= 1 ? 1 : x);
}

bool TextureMaterial::load(std::string filename)
{
    Mat img = imread(filename);
    if(img.data == 0)
    {
        std::cout << "Error loading texture \"" << filename << "\"" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Successfully loaded texture \"" << filename << "\"" << std::endl;
    }
    img.convertTo(texture, CV_32F, 1./255.);
    return true;
}

cv::Vec3f TextureMaterial::shade(const HitRecord & hit, QVector3D) const
{
    return get(hit.getTexCoords()) * (1. / M_PI);
}

cv::Vec3f TextureMaterial::get(Point2f location) const
{
    return texture.at<Vec3f>(Point2i(clamp(location.x) * (texture.size().height - 1), clamp(location.y) * (texture.size().width - 1)));
}
