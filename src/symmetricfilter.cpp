#include "symmetricfilter.h"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

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

    Mat paddedImage = Mat::zeros(image.size() + Size(2 * padding, 2 * padding), image.type());
    Mat paddedImageSubheader = paddedImage(Range(padding, padding + image.size().height), Range(padding, padding + image.size().width));
    assert(paddedImageSubheader.size() == image.size());
    image.copyTo(paddedImageSubheader);

    Mat paddedGuide = Mat::zeros(guide.size() + Size(2 * padding, 2 * padding), guide.type());
    Mat paddedGuideSubheader = paddedGuide(Range(padding, padding + guide.size().height), Range(padding, padding + guide.size().width));
    assert(paddedGuideSubheader.size() == guide.size());
    guide.copyTo(paddedGuideSubheader);

    for(int dx = -windowRadius; dx < windowRadius; dx++)
    {
        for(int dy = -windowRadius; dy < windowRadius; dy++)
        {
            source = paddedGuide(Range(padding + dy, padding + dy + guide.size().height), Range(padding + dx, padding + dx + guide.size().width));
            data = paddedImage(Range(padding + dy, padding + dy + guide.size().height), Range(padding + dx, padding + dx + image.size().width));
            diff = source - image;
            blur(diff.mul(diff), d2, Size(patchSize, patchSize));
            Mat doubleVar = 2 * pixvar;
            Mat maxArg = d2 - doubleVar;
            Mat maximum = max(maxArg, 0);
            exp(-maximum / (h2 * pixvar), weights);

            area += weights;
            acc += weights.mul(data);
        }
    }

    return acc / area;
}
