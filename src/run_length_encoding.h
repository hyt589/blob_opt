#pragma once

#include <stdint.h>
#include <vector>

namespace cv {
class Mat;
}

class Region final {
public:
  Region();
  Region(const Region &region);
  Region &operator=(const Region &region);
  virtual ~Region();

public:
  int16_t *run_x_pairs;
  int16_t *run_y;
  uint64_t run_count = 0;

  std::vector<int> run_labels;

  int image_width = -1;
  int image_height = -1;

  uint64_t num_labels = 0;
  std::vector<uint64_t> area;
  std::vector<int16_t> top_left_x;
  std::vector<int16_t> top_left_y;
  std::vector<int16_t> bottom_right_x;
  std::vector<int16_t> bottom_right_y;
  std::vector<float> centroid_x;
  std::vector<float> centroid_y;

private:
  uint64_t *m_ref_count = nullptr;
};

void ThresholdAndEncode(const cv::Mat image, Region &encoded_region,
                        const uint8_t lo, const uint8_t hi);
void Encode(const cv::Mat image, Region &dst);
void Decode(const Region &region, cv::Mat &dst);
