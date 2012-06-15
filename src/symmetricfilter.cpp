#include "symmetricfilter.h"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

cv::Mat padArray(const cv::Mat &array, int padding)
{
    Mat paddedArray = Mat::zeros(array.size() + Size(2 * padding, 2 * padding), array.type());
    Mat paddedArraySubheader = paddedArray(Range(padding, padding + array.size().height), Range(padding, padding + array.size().width));
    array.copyTo(paddedArraySubheader);
    return paddedArray;
}

cv::Mat SymmetricFilter::filter(const cv::Mat &image, const cv::Mat &guide, const cv::Mat &pixvar)
{
    Mat d2, source, diff, weights, data;
    Mat area = Mat::zeros(image.size(), image.type());
    Mat acc = Mat::zeros(image.size(), image.type());

    const int patchSize = 7;
    const int windowRadius = 10;

    const int padding = windowRadius;

    const float h = 0.4f;
    const float h2 = h * h;

    Mat paddedImage = padArray(image, padding);

    Mat paddedGuide = padArray(guide, padding);

    for(int dx = -windowRadius; dx <= windowRadius; dx++)
    {
        for(int dy = -windowRadius; dy <= windowRadius; dy++)
        {
            source = paddedGuide(Range(padding + dy, padding + dy + guide.size().height), Range(padding + dx, padding + dx + guide.size().width));
            data = paddedImage(Range(padding + dy, padding + dy + guide.size().height), Range(padding + dx, padding + dx + image.size().width));
            diff = source - image;
            blur(diff.mul(diff), d2, Size(patchSize, patchSize), Point(-1, -1), BORDER_CONSTANT);
            exp(-max(d2 - 2 * pixvar, 0) / (h2 * pixvar), weights);

            area += weights;
            acc += weights.mul(data);
        }
    }

    return acc / area;
}
