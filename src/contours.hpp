/** @file */

#pragma once

#include "utils.hpp"
#include <utility>
#include <vector>

/**
 * A function to get the contours of the union of rectangles
 * @return vector of pair of points, where the pair represents 2 ends of a line
 * segment
 */
std::vector<std::pair<Point, Point>>
getContours(std::vector<Rectangle> rectangles);
