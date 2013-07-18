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
#include "symmetricfilter.h"

#include "global.h"
#include "mathhelper.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <algorithm>
#include <vector>
#include <iostream>

#include <QString>

using namespace cv;
using namespace std;

cv::Mat padArray(const cv::Mat &array, int padding)
{
    Mat paddedArray = Mat::zeros(array.size() + Size(2 * padding, 2 * padding), array.type());
    Mat paddedArraySubheader = paddedArray(Range(padding, padding + array.size().height), Range(padding, padding + array.size().width));
    array.copyTo(paddedArraySubheader);
    return paddedArray;
}

cv::Mat computeWeights(const Mat& source, const Mat& target, const Mat& var, const Mat& shiftedVar, int patchSize, float h2)
{
    assert(checkRange(source));
    assert(checkRange(target));
    assert(checkRange(var));
    assert(checkRange(shiftedVar));

    Mat weights, temp, temp2, temp3, temp4, temp5, diff, d2;
    diff = source - target;
    Mat sqdiff = diff.mul(diff);
    d2 = channelMean(sqdiff - (var + min(shiftedVar, var)));
    assert(checkRange(d2));
    Mat d2_blurred;
    blur(d2, d2_blurred, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
    temp2 = -max(d2_blurred, 0);
    temp3 = 1e-8f * Mat::ones(source.size(), CV_32F);
    temp4 = h2 * channelMean(var + shiftedVar);
    Mat expin = temp2 / (temp3 + temp4);
    assert(checkRange(expin));
    exp(expin, temp5);
    blur(temp5, temp, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
    weights = max(temp, 0);
    assert(checkRange(weights));
    assert(weights.channels() == 1);
    Mat idx = weights < 0.05;
    add(Mat::zeros(weights.size(), weights.type()), 0, weights, idx);
    return weights;
}

cv::Mat SymmetricFilter::filter(const cv::Mat &image, const cv::Mat &guide, const cv::Mat &pixvar)
{
    assert(checkRange(image));
    Mat area = Mat::zeros(image.size(), CV_32F);
    Mat acc = Mat::zeros(image.size(), image.type());


    const int padding = windowRadius;

    const float h = 0.4f;
    const float h2 = h * h;

    Mat paddedImage = padArray(image, padding);

    Mat paddedGuide = padArray(guide, padding);

    Mat paddedVariance = padArray(pixvar, padding);

#pragma omp parallel for
    for(int dx = -windowRadius; dx <= 0; dx++)
    {
        for(int dy = -windowRadius; dy <= windowRadius; dy++)
        {
            Range yrange1(padding - dy, padding - dy + guide.size().height);
            Range xrange1(padding - dx, padding - dx + guide.size().width);
            Range yrange2(padding + dy, padding + dy + guide.size().height);
            Range xrange2(padding + dx, padding + dx + guide.size().width);
            Mat source1 = paddedGuide(yrange1, xrange1);
            Mat source2 = paddedGuide(yrange2, xrange2);
            Mat source_S = (source1 + source2) / 2.;
            Mat data1 = paddedImage(yrange1, xrange1);
            Mat data2 = paddedImage(yrange2, xrange2);
            Mat var1 = paddedVariance(yrange1, xrange1);
            Mat var2 = paddedVariance(yrange2, xrange2);
            Mat varS = (var1 + var2) / 4;


            if(dx == 0 && dy == 0)
            {
#pragma omp critical
                {
                    add(area, Scalar(Vec3f(1, 1, 1)), area);
                    acc += data1; // data1 == data2
                }
                break;
            }

            Mat weights1 = computeWeights(source1, guide, pixvar, var1, patchSize, h2);
            Mat weights2 = computeWeights(source2, guide, pixvar, var2, patchSize, h2);
            Mat weightsS = computeWeights(source_S, guide, pixvar, varS, patchSize, h2);

            Mat idx = weightsS > max(weights1, weights2);
            add(weightsS, 0, weights1, idx);
            add(weightsS, 0, weights2, idx);

            Mat totalWeights = weights1 + weights2;
            Mat totalData = extend(weights1).mul(data1) + extend(weights2).mul(data2);
#pragma omp critical
            {
                area += totalWeights;
                acc += totalData;
                assert(checkRange(area));
                assert(checkRange(acc));
            }
        }
    }
    return acc / extend(area);
}
