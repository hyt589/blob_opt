#include "clock.h"
#include "defines.h"
#include "labeling.h"
#include "opencv2/core/types.hpp"
#include "run_length_encoding.h"
#include "show_image_fit_screen.h"
#include <iostream>
#include <opencv2/opencv.hpp>

#include <stdint.h>
#include <vector>

void DrawBBoxes(cv::Mat canvas, const Region &rle) {
  for (uint64_t i = 0; i < rle.num_labels; i++) {
    cv::Point2f p1(rle.top_left_x[i], rle.top_left_y[i]);
    cv::Point2f p2(rle.bottom_right_x[i], rle.bottom_right_y[i]);
    cv::rectangle(canvas, cv::Rect(p1, p2), cv::Scalar{255, 255, 0}, 5);
  }
}
void DrawCentroids(cv::Mat canvas, const Region &rle) {
  for (uint64_t i = 0; i < rle.num_labels; i++) {
    cv::Point2f p(rle.centroid_x[i], rle.centroid_y[i]);
    cv::circle(canvas, p, 3, cv::Scalar(0, 255, 255), -1);
  }
}

int main(int argc, char **argv) {
  auto image = cv::imread("E:/blob-optimize/data/Image_20230228163916267.bmp",
                          cv::IMREAD_GRAYSCALE);

  ImShowFitScreen(image, "grayscale");
  cv::waitKey();

  Region rle;
  {
    Clock c("Full");
    {
      Clock clock("Encoding");
      ThresholdAndEncode(image, rle, 0, 127);
    }

    {
      Clock clock("labeling");
      rle.num_labels = RunBasedLabeling(rle);
    }

    {
      Clock clock("FC");
      ComputeStatsAfter(rle);
    }
  }

  cv::Mat decoded;

  {
    Clock clock("Decoding");
    Decode(rle, decoded);
  }

  DrawBBoxes(decoded, rle);
  DrawCentroids(decoded, rle);

  ImShowFitScreen(decoded, "decoded");
  cv::waitKey();
}
