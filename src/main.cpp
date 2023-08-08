#include "clock.h"
#include "defines.h"
#include "labeling.h"
#include "opencv2/core/types.hpp"
#include "run_length_encoding.h"
#include "show_image_fit_screen.h"
#include <iostream>
#include <opencv2/opencv.hpp> #include <stdint.h>
#include <vcruntime.h>
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
    cv::circle(canvas, p, 3, cv::Scalar(0, 0, 255), -1);
  }
}

void func() {
  cv::Mat1b mat(2, 13);
  mat << 255, 240, 255, 0, 255, 0, 0, 255, 255, 240, 255, 0, 0, 0, 0, 255, 0, 0,
      255, 0, 0, 255, 0, 0, 255, 255;
  /* std::cout << mat << std::endl; */
  PRINT_VAL(mat);

  Region r;
  const int iter = 100;
  {
    // print result
    ThresholdAndEncode(mat, r, 127, 256);
  }

  PRINT_LOC();
  cv::Mat m_out = cv::Mat::zeros(mat.rows, mat.cols, CV_8UC1);
  PRINT_LOC();

  Decode(r, m_out);
  PRINT_LOC();
  /* simdblob::PaintRegion(r, m_out, m_out, 255,
   * simdblob::PaintRegionType::kFill); */
  PRINT_VAL(m_out);
}

void func2() {
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

    PRINT_VAL(rle.run_count);

    {
      Clock clock("Labeling");
      rle.num_labels = RunBasedLabeling(rle);
    }

    PRINT_VAL(rle.num_labels);

    {
      Clock clock("FC");
      ComputeStatsAfter(rle);
    }

    {
      Clock clock("Sort");
      SortLabels(rle, Metric::kArea);
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

void func3() {
  std::vector<cv::Point2f> points{
      {1526.38, 309.433}, {1562.95, 323.03},  {1599.68, 336.138},
      {1636.41, 349.278}, {1673.23, 362.169}, {1709.84, 375.635},
      {1746.64, 388.552}, {1783.35, 401.755}, {1820.07, 414.898},
      {1856.74, 428.212}, {1893.31, 441.779}, {1930.1, 454.735},
      {1966.77, 468.022}, {2003.39, 481.463}, {2040.04, 494.815},
      {2076.86, 507.712}, {2113.54, 520.981}, {2150.17, 534.374},
      {2186.95, 547.383}, {2223.6, 560.736},  {2260.23, 574.145},
      {2297.08, 586.937}, {2333.66, 600.477}, {2370.33, 613.787},
      {2407.05, 626.945}, {2443.74, 640.173}, {2480.48, 653.283},
      {2517.21, 666.418}, {2553.93, 679.581}, {2590.52, 693.091},
  };
  cv::Vec4f line;
  {
    Clock fit("Fit");
    cv::fitLine(points, line, cv::DIST_L2, 0, 1e-2, 1e-2);
  }
  Clock clk("C(60, 30)");
  for (size_t i = 0; i < 118264581564861424; i++) {
    /* cv::fitLine(points, line, cv::DIST_L2, 0, 1e-2, 1e-2); */
  }
}

int main(int argc, char **argv) {
  /* func(); */
  func2();
  /* func3(); */
}
