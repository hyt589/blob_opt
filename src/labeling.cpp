#include "labeling.h"
#include "clock.h"
#include "union_find.h"

#include <algorithm>
#include <execution>
#include <limits>
#include <stdint.h>
#include <type_traits>
#include <vector>

namespace {
int LabelRuns(const int16_t *run_x_pairs, const int16_t *run_y, int len,
              int *labels, int offset) {
  int idxLabel = 1;
  int curRowIdx = 0;
  int firstRunOnCur = 0;
  int firstRunOnPre = 0;
  int lastRunOnPre = -1;
  int NumberOfRuns = len;

  // 完成团的标记和等价对列表的生成,将P改成等价对中最小的值
  int *P = new int[NumberOfRuns];
  // first label is for background pixels
  P[0] = 0;

  for (int i = 0; i < NumberOfRuns; i++) {
    if (run_y[i] != curRowIdx) {
      curRowIdx = run_y[i];
      firstRunOnPre = firstRunOnCur;
      lastRunOnPre = i - 1;
      firstRunOnCur = i;
    }

    auto &run_x_start = run_x_pairs[i * 2];
    auto &run_x_end = run_x_pairs[i * 2 + 1];

    for (int j = firstRunOnPre; j <= lastRunOnPre; j++) {
      auto &pre_run_x_start = run_x_pairs[j * 2];
      auto &pre_run_x_end = run_x_pairs[j * 2 + 1];
      if (run_x_start <= pre_run_x_end + offset &&
          run_x_end >= pre_run_x_start - offset && run_y[i] == run_y[j] + 1) {
        if (labels[i] == 0) // 没有被标号过
          labels[i] = labels[j];
        else if (labels[i] != labels[j]) // 已经被标号
          labels[i] = SetUnion(P, labels[i], labels[j]);
      }
    }
    if (labels[i] == 0) // 没有与前一列的任何run重合
    {
      P[idxLabel] = idxLabel;
      labels[i] = idxLabel++;
    }
  }

  // analysis 重新排序
  int nLabels = Flatten(P, idxLabel);

  for (int i = 0; i < len; ++i) {
    // int & l = runLabels.at(i);
    int l = labels[i];
    labels[i] = P[l];
    // l = P[l];
  }

  delete[] P;
  return nLabels;
}

void ComputeLabelStatsSequential(Region &region) {
  Clock clk("Features");

  region.area.assign(region.num_labels, 0);
  region.top_left_x.assign(region.num_labels,
                           std::numeric_limits<int16_t>::max());
  region.top_left_y.assign(region.num_labels,
                           std::numeric_limits<int16_t>::max());
  region.bottom_right_x.assign(region.num_labels, 0);
  region.bottom_right_y.assign(region.num_labels, 0);
  region.centroid_x.assign(region.num_labels, 0);
  region.centroid_y.assign(region.num_labels, 0);

  for (uint64_t i = 0; i < region.run_count; i++) {
    const auto label = region.run_labels.data()[i];
    const auto pair = region.run_x_pairs + i * 2;

    auto &area = region.area.data()[label];
    auto &tl_x = region.top_left_x.data()[label];
    auto &tl_y = region.top_left_y.data()[label];
    auto &br_x = region.bottom_right_x.data()[label];
    auto &br_y = region.bottom_right_y.data()[label];
    auto &cx = region.centroid_x.data()[label];
    auto &cy = region.centroid_y.data()[label];

    auto s = pair[1] - pair[0] + 1;
    auto sx = (pair[0] + pair[1]) / 2.f;
    auto sy = region.run_y[i];

    cx = (area * cx + s * sx) / (area + s);
    cy = (area * cy + s * sy) / (area + s);

    tl_x = std::min(tl_x, pair[0]);
    tl_y = std::min(tl_y, region.run_y[i]);
    br_x = std::max(br_x, pair[1]);
    br_y = std::max(br_y, region.run_y[i]);
    area += s;
  }
}

} // namespace

int RunBasedLabeling(Region &region) {
  region.run_labels.assign(region.run_count, 0);
  return LabelRuns(region.run_x_pairs, region.run_y, region.run_count,
                   region.run_labels.data(), 1);
}

void ComputeStatsAfter(Region &region) { ComputeLabelStatsSequential(region); }
