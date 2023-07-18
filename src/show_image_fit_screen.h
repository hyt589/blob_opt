#pragma once
#include <string>

namespace cv {
class Mat;
}

void ImShowFitScreen(const cv::Mat image, const std::string &name = "imshow");
