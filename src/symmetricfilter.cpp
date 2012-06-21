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
    Mat out;
    cvtColor(outSingle, out, CV_GRAY2BGR);
    imwrite("/tmp/outsingle.exr", outSingle);
    return out;
//    return in;
}

cv::Mat SymmetricFilter::filter(const cv::Mat &image, const cv::Mat &guide, const cv::Mat &pixvar)
{
    Mat d2, source1, source2, source_est_S, diff, weights1, weights2, weights_S, weights, data1, data2, var1, var2, var_S;
    Mat area = Mat::zeros(image.size(), image.type());
    Mat acc = Mat::zeros(image.size(), image.type());
    Mat temp;

    const int patchSize = 7;
    const int windowRadius = 10;

    const int padding = windowRadius;

    const float h = 0.4f;
    const float h2 = h * h;

    Mat paddedImage = padArray(image, padding);

    Mat paddedGuide = padArray(guide, padding);

    Mat paddedVariance = padArray(pixvar, padding);

    for(int dy = -windowRadius; dy <= 0; dy++)
    {
        for(int dx = -windowRadius; dx <= windowRadius; dx++)
        {
            source1 = paddedGuide(Range(padding + dy, padding + dy + guide.size().height), Range(padding + dx, padding + dx + guide.size().width));
            source2 = paddedGuide(Range(padding - dy, padding - dy + guide.size().height), Range(padding - dx, padding - dx + guide.size().width));
            source_est_S = (source1 + source2) / 2.;
            data1 = paddedImage(Range(padding + dy, padding + dy + guide.size().height), Range(padding + dx, padding + dx + image.size().width));
            data2 = paddedImage(Range(padding - dy, padding - dy + guide.size().height), Range(padding - dx, padding - dx + image.size().width));
            var1 = paddedVariance(Range(padding + dy, padding + dy + guide.size().height), Range(padding + dx, padding + dx + image.size().width));
            var2 = paddedVariance(Range(padding - dy, padding - dy + guide.size().height), Range(padding - dx, padding - dx + image.size().width));
            var_S = (var1 + var2) / 4;

            if(dx == 0 && dy == 0)
            {
                area += 1;
                acc += data1;
                break;
            }

            diff = source1 - guide;
            blur(diff.mul(diff), d2, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
//            exp(-max(d2 - 2 * pixvar, 0) / (1e-10f + h2 * pixvar), weights);
            //use min(var1, pixvar)
//            exp(-(d2 - (pixvar + min(var1, pixvar))) / (h2 * (pixvar + var1)), temp);
            exp(-(d2 - (pixvar + var1)) / (h2 * (pixvar + var1)), weights);
//            blur(temp, weights, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
            //threshold?
            weights1 = max(channelMean(weights), 0);

            diff = source2 - guide;
            blur(diff.mul(diff), d2, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
//            exp(-max(d2 - 2 * pixvar, 0) / (1e-10f + h2 * pixvar), weights);
//            exp(-(d2 - (pixvar + min(var2, pixvar))) / (h2 * (pixvar + var2)), temp);
            exp(-(d2 - (pixvar + var2)) / (h2 * (pixvar + var2)), weights);
//            blur(temp, weights, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
            weights2 = max(channelMean(weights), 0);

            diff = source_est_S - guide;
            blur(diff.mul(diff), d2, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
//            exp(-max(d2 - 2 * pixvar, 0) / (1e-10f + h2 * pixvar), weights);
//            exp(-(d2 - (pixvar + min(var_S, pixvar))) / (h2 * (pixvar + var_S)), temp);
            exp(-(d2 - (pixvar + var_S)) / (h2 * (pixvar + var_S)), weights);
//            blur(temp, weights, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
            weights_S = max(channelMean(weights), 0);

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

            area += weights1 + weights2;
            acc += weights1.mul(data1) + weights2.mul(data2);
        }
    }

    return acc / area;
}
