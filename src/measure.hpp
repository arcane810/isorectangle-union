/** @file */

#pragma once

#include "utils.hpp"
#include <utility>
#include <vector>

/**
 * A class so store the stripes for contours
 */
class MeasureStripes {
  public:
    /// The x interval of the stripe
    XInterval x_interval;
    /// The y interval of the stripe
    YInterval y_interval;
    /// the length of x_union
    long double x_union;
    /**
     * Constructor.
     *
     * @param x_interval The x interval of the stripe
     * @param y_interval The y interval of the stripe
     * @param x_union The length of x_union
     */
    MeasureStripes(XInterval x_interval, YInterval y_interval,
                   long double x_union);
};

/**
 * A function to get the measure of the union of rectangles
 * @return area of union of rectangles
 */
long double getMeasure(std::vector<Rectangle> &rectangles);
