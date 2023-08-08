#include "run_length_encoding.h"
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include <tuple>
#include <unordered_map>

namespace {

struct LabelRuns {
  int16_t *run_x_pairs;
  int16_t *run_y;
  int label;
  uint64_t len;

  LabelRuns() {}
};

struct LineRuns {
  int16_t *run_x_pairs;
  uint64_t len;
};

LabelRuns SeperateRegionByLabel(const Region &rle) {
  LabelRuns runs;
  return runs;
}

} // namespace
