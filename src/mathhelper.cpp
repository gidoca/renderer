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

    mean = sum * (1. / in.size());
    var = max((sqr_sum - sum.mul(mean)) / (in.size() - 1), 0);
}

void mean(Mat &out, vector<Mat> in)
{
    Mat sum = Mat::zeros(in[0].size(), in[0].type());
    for(unsigned int i = 0; i < in.size(); i++)
    {
      sum += in[i];
    }

    out = sum * (1. / in.size());
}

cv::Mat channelMean(const cv::Mat &in)
{
    assert(checkRange(in));
    vector<Mat> splitted;
    Mat outSingle = Mat::zeros(in.size(), CV_MAKETYPE(in.type(), 1));
    split(in, splitted);
    for(unsigned int i = 0; i < splitted.size(); i++)
    {
        outSingle += splitted[i] / splitted.size();
    }
    assert(checkRange(outSingle));
    return outSingle;
//    return in;
}

cv::Mat channelNorm(const Mat &in)
{
    vector<Mat> splitted;
    Mat outSquared = Mat::zeros(in.size(), CV_MAKETYPE(in.type(), 1)), out;
    split(in, splitted);
    for(unsigned int i = 0; i < splitted.size(); i++)
    {
        outSquared += splitted[i].mul(splitted[i]);
    }
    sqrt(outSquared, out);
    return out;
}

cv::Mat extend(const cv::Mat &in, int nchan)
{
    Mat out;
    merge(vector<Mat>(nchan, in), out);
    return out;
}
