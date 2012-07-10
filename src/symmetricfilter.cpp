#include "symmetricfilter.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <algorithm>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

cv::Mat padArray(const cv::Mat &array, int padding)
{
    Mat paddedArray = Mat::zeros(array.size() + Size(2 * padding, 2 * padding), array.type());
    Mat paddedArraySubheader = paddedArray(Range(padding, padding + array.size().height), Range(padding, padding + array.size().width));
    array.copyTo(paddedArraySubheader);
    return paddedArray;
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

cv::Mat computeWeights(const Mat& source, const Mat& target, const Mat& var, const Mat& shiftedVar, int patchSize, float h2)
{
    assert(checkRange(source));
    assert(checkRange(target));
    Mat weights, temp, temp2, diff, d2;
    diff = source - target;
//    Mat sqdiff = channelMean(diff);
    Mat sqdiff = diff.mul(diff);
    blur(sqdiff, d2, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
    assert(checkRange(sqdiff));
//            exp(-max(d2 - 2 * pixvar, 0) / (1e-10f + h2 * pixvar), weights);
    //use min(var1, pixvar)
    exp(-max(channelMean(d2 - (var + min(shiftedVar, var))), 0) / (1e-8f * Mat::ones(source.size(), CV_32F) + h2 * channelMean(var + shiftedVar)), temp);
//    exp(-(d2 - channelMean(var + shiftedVar)) / (1e-8f + h2 * channelMean(var + shiftedVar)), temp);
//    exp(-(d2 - (var + shiftedVar)) / (1e-8f * Mat::ones(source.size(), source.type()) + h2 * (var + shiftedVar)), temp);
    blur(temp, temp2, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
    //threshold?
//    weights = max(channelMean(temp2), 0);
    weights = max(temp2, 0);
    assert(checkRange(weights));
    assert(weights.channels() == 1);
    Mat out(source.size(), source.type());
    int channelMapping[] = {0, 0, 0, 1, 0, 2};
    mixChannels(&weights, 1, &out, 1, channelMapping, 3);
    return out;
//    return weights;
}

cv::Mat SymmetricFilter::filter(const cv::Mat &image, const cv::Mat &guide, const cv::Mat &pixvar)
{
    assert(checkRange(image));
    Mat area = Mat::zeros(image.size(), image.type());
    Mat acc = Mat::zeros(image.size(), image.type());

    const int patchSize = 7;
    const int windowRadius = 10;

    const int padding = windowRadius;

    const float h = 0.4f;
    const float h2 = h * h;

    Mat paddedImage = padArray(image, padding);

    Mat paddedGuide = padArray(guide, padding);

    Mat paddedVariance = padArray(pixvar, padding);

#pragma omp parallel for
    for(int dy = -windowRadius; dy <= 0; dy++)
    {
        for(int dx = -windowRadius; dx <= windowRadius; dx++)
        {
            Mat source1 = paddedGuide(Range(padding + dy, padding + dy + guide.size().height), Range(padding + dx, padding + dx + guide.size().width));
            Mat source2 = paddedGuide(Range(padding - dy, padding - dy + guide.size().height), Range(padding - dx, padding - dx + guide.size().width));
            Mat source_S = (source1 + source2) / 2.;
            Mat data1 = paddedImage(Range(padding + dy, padding + dy + guide.size().height), Range(padding + dx, padding + dx + image.size().width));
            Mat data2 = paddedImage(Range(padding - dy, padding - dy + guide.size().height), Range(padding - dx, padding - dx + image.size().width));
            Mat var1 = paddedVariance(Range(padding + dy, padding + dy + guide.size().height), Range(padding + dx, padding + dx + image.size().width));
            Mat var2 = paddedVariance(Range(padding - dy, padding - dy + guide.size().height), Range(padding - dx, padding - dx + image.size().width));
            Mat var_S = (var1 + var2) / 4;


            if(dx == 0 && dy == 0)
            {
#pragma omp critical
                {
                    area += 1;
                    acc += data1;
                }
                break;
            }

            Mat weights1 = computeWeights(source1, guide, pixvar, var1, patchSize, h2);
            Mat weights2 = computeWeights(source2, guide, pixvar, var2, patchSize, h2);
            Mat weights_S = computeWeights(source_S, guide, pixvar, var_S, patchSize, h2);

            vector<Mat> weights1_split, weights2_split, weights_S_split;
            split(weights1, weights1_split);
            split(weights2, weights2_split);
            split(weights_S, weights_S_split);
            Mat idx = Mat::zeros(weights1.size(), CV_8UC1);
            for(unsigned int i = 0; i< weights1_split.size(); i++)
            {
                idx |= weights_S_split[i] > max(weights1_split[i], weights2_split[i]);
            }
            add(weights_S, 0, weights1, idx);
            add(weights_S, 0, weights2, idx);

            Mat totalWeights = weights1 + weights2;
            Mat totalData = weights1.mul(data1) + weights2.mul(data2);
#pragma omp critical
            {
                area += totalWeights;
                acc += totalData;
                assert(checkRange(area));
                assert(checkRange(acc));
            }
        }
    }

    return acc / area;
}
