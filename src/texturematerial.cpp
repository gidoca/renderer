#include "texturematerial.h"

#include "hitrecord.h"

#include <opencv2/highgui/highgui.hpp>

#include <cmath>
#include <iostream>

using namespace cv;

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
    return texture.at<Vec3f>(Point2i(location.x * (texture.size().height - 1), location.y * (texture.size().width - 1)));
}
