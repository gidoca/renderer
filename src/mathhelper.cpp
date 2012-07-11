#include "mathhelper.h"

using namespace std;
using namespace cv;

void var(Mat &mean, Mat &var, vector<Mat> in)
{
    Mat sum = Mat::zeros(in[0].size(), in[0].type()), sqr_sum = Mat::zeros(in[0].size(), in[0].type());

    for(unsigned int i = 0; i < in.size(); i++)
    {
      sum += in[i];
      sqr_sum += in[i].mul(in[i]);
    }

    mean.create(sum.size(), sum.type());
    var.create(sum.size(), sum.type());

    mean = sum * (1. / in.size());
    var = max((sqr_sum - sum.mul(mean)) / (in.size() - 1), 0);
}

cv::Mat channelMean(const cv::Mat &in)
{
    vector<Mat> splitted;
    Mat outSingle = Mat::zeros(in.size(), CV_MAKETYPE(in.type(), 1));
    split(in, splitted);
    for(unsigned int i = 0; i < splitted.size(); i++)
    {
        outSingle += splitted[i] / splitted.size();
    }
    return outSingle;
//    return in;
}

