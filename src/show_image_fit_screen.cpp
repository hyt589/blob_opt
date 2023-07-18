#include "show_image_fit_screen.h"
#include "opencv2/imgproc.hpp"

#include <opencv2/opencv.hpp>

const static int kScreenWidth = 1920;
const static int kScreenHeight = 1080;
const static float kRatio = 0.8;

void ImShowFitScreen(const cv::Mat image, const std::string &name) {
  cv::Mat resized = image.clone();
  if (resized.size().height > kScreenHeight) {
    auto factor = static_cast<float>(kScreenHeight) * kRatio /
                  static_cast<float>(resized.size().height);
    cv::resize(resized, resized,
               cv::Size(resized.size().width * factor,
                        resized.size().height * factor));
  }
  if (resized.size().width > kScreenWidth) {
    auto factor = static_cast<float>(kScreenWidth) * kRatio /
                  static_cast<float>(resized.size().width);
    cv::resize(
        resized, resized,
        cv::Size(image.size().width * factor, image.size().height * factor));
  }
  cv::imshow(name, resized);
}
