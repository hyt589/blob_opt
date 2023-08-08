#pragma once

#include "run_length_encoding.h"
#include <vector>

int RunBasedLabeling(Region &region);

void ComputeStatsAfter(Region &region);

enum class Metric {
  kArea = 0,
  kCentroidX,
  kCentroidY,
  kWidth,
  kHeight,
};

std::vector<uint64_t> SortLabels(const Region &region, const Metric metric);
