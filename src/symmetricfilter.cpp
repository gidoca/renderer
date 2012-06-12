#include "symmetricfilter.h"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

cv::Mat SymmetricFilter::filter(const cv::Mat &in, const cv::Mat &guide)
{
  Mat d2(in.size(), in.type());
  Mat weight(in.size(), in.type());

  const int patchSize = 7;
  const int windowRadius = 10;

  const int windowSize = windowRadius * 2 + 1;
  const int patchRadius = (patchSize - 1) / 2;

  const int padding = windowRadius + patchRadius;

  float h = 0.4f;

  Mat paddedGuide = Mat::zeros(guide.size() + Size(2 * padding, 2 * padding), CV_32FC3);
  Mat paddedGuideSubheader = paddedGuide(Range(padding, padding + guide.size().height - 1), Range(padding, padding + guide.size().width - 1));
  guide.copyTo(paddedGuideSubheader);

  for(int dx = -windowRadius; dx < windowRadius; dx++)
  {
    for(int dy = -windowRadius; dy < windowRadius; dy++)
    {
      Mat data;
      Mat diff;
      blur(in, d2, Size(patchSize, patchSize));
    }
  }

  return d2;
}
