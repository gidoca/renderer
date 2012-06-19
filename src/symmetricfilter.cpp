#include "symmetricfilter.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <algorithm>
#include <vector>

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
    Mat outSingle = Mat::zeros(in.size(), CV_32F);
    split(in, splitted);
    for(unsigned int i = 0; i < splitted.size(); i++)
    {
        outSingle += splitted[i] / splitted.size();
    }
    Mat out;
    merge(vector<Mat>(3, outSingle), out);
    return out;
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

            diff = /*channelMean*/(source1 - guide);
            blur(diff.mul(diff), d2, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
//            exp(-max(d2 - 2 * pixvar, 0) / (1e-10f + h2 * pixvar), weights);
            //use min(var1, pixvar)
//            exp(-(d2 - (pixvar + min(var1, pixvar))) / (h2 * (pixvar + var1)), temp);
            exp(-(d2 - (pixvar + var1)) / (h2 * (pixvar + var1)), temp);
            blur(temp, weights, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
            //threshold?
            weights1 = max(weights, 0);

            diff = /*channelMean*/(source2 - guide);
            blur(diff.mul(diff), d2, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
//            exp(-max(d2 - 2 * pixvar, 0) / (1e-10f + h2 * pixvar), weights);
//            exp(-(d2 - (pixvar + min(var2, pixvar))) / (h2 * (pixvar + var2)), temp);
            exp(-(d2 - (pixvar + var2)) / (h2 * (pixvar + var2)), temp);
            blur(temp, weights, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
            weights2 = max(weights, 0);

            diff = channelMean(source_est_S - guide);
            blur(diff.mul(diff), d2, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
//            exp(-max(d2 - 2 * pixvar, 0) / (1e-10f + h2 * pixvar), weights);
//            exp(-(d2 - (pixvar + min(var_S, pixvar))) / (h2 * (pixvar + var_S)), temp);
            exp(-(d2 - (pixvar + var_S)) / (h2 * (pixvar + var_S)), temp);
            blur(temp, weights, Size(patchSize, patchSize), Point(-1, -1), BORDER_REFLECT);
            weights_S = max(weights, 0);

            for(int i = 0; i < weights1.size().height; i++)
            {
                for(int j = 0; j < weights1.size().width; j++)
                {
                    for(int k = 0; k < 3; k++)
                    {
                        float &w1 = weights1.at<Vec3f>(i, j)[k];
                        float &w2 = weights2.at<Vec3f>(i, j)[k];
                        float &wS = weights_S.at<Vec3f>(i, j)[k];
//                        float threshold = 0.05f;
//                        if(w1 < threshold) w1 = 0;
//                        if(w2 < threshold) w2 = 0;
//                        if(wS < threshold) wS = 0;
                        if(wS > max(w1, w2))
                        {
                            w1 = wS;
                            w2 = wS;
//                        }
                    }
                }
            }

            area += weights1 + weights2;
            acc += weights1.mul(data1) + weights2.mul(data2);
        }
    }

    return acc / area;
}
