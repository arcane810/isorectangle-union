/** @file */

#pragma once

#include "utils.hpp"
#include <utility>
#include <vector>

class ContourStripes {
  public:
    XInterval x_interval;
    YInterval y_interval;
    std::vector<XInterval> x_union;

    ContourStripes(XInterval x_interval, YInterval y_interval,
                   std::vector<XInterval> x_union);
};

/**
 * A function to get the contours of the union of rectangles
 * @return vector of pair of points, where the pair represents 2 ends of a line
 * segment
 */
std::vector<std::pair<Point, Point>>
getContours(std::vector<Rectangle> rectangles);
