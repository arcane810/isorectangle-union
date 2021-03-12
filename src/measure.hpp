/** @file */

#pragma once

#include "utils.hpp"
#include <utility>
#include <vector>

class MeasureStripes {
  public:
    XInterval x_interval;
    YInterval y_interval;
    long double x_union;

    MeasureStripes(XInterval x_interval, YInterval y_interval,
                   long double x_union);
};

/**
 * A function to get the measure of the union of rectangles
 * @return area of union of rectangles
 */
long double getMeasure(std::vector<Rectangle> rectangles);
