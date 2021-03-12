/** @file */

#pragma once

#include "utils.hpp"
#include <utility>
#include <vector>

enum NodeEdgeType { LEFT_U, RIGHT_U, UNDEF };

class UnionTree {
  public:
    long double coord;
    NodeEdgeType node_edge_type;
    UnionTree *left;
    UnionTree *right;
    UnionTree(long double coord, NodeEdgeType node_edge_type, UnionTree *left,
              UnionTree *right);
};

class ContourStripes {
  public:
    XInterval x_interval;
    YInterval y_interval;
    UnionTree *x_union;

    ContourStripes(XInterval x_interval, YInterval y_interval,
                   UnionTree *x_union);
};

/**
 * A function to get the contours of the union of rectangles
 * @return vector of pair of points, where the pair represents 2 ends of a line
 * segment
 */
std::vector<std::pair<Point, Point>>
getContours(std::vector<Rectangle> rectangles);
