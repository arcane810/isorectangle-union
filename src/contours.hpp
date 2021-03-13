/** @file */

#pragma once

#include "utils.hpp"
#include <utility>
#include <vector>

/**
 * An enum to denote the edge type of values in BST
 */
enum NodeEdgeType { LEFT_U, RIGHT_U, UNDEF };

/**
 * BST for storing x_union of stripes
 */
class UnionTree {
  public:
    long double coord;
    NodeEdgeType node_edge_type;
    UnionTree *left;
    UnionTree *right;
    UnionTree(long double coord, NodeEdgeType node_edge_type, UnionTree *left,
              UnionTree *right);
};

/**
 * A class so store the stripes for contours
 */
class ContourStripes {
  public:
    /// The x interval of the stripe
    XInterval x_interval;
    /// The y interval of the stripe
    YInterval y_interval;
    /// the BST for x_union
    UnionTree *x_union;

    /**
     * Constructor.
     *
     * @param x_interval The x interval of the stripe
     * @param y_interval The y interval of the stripe
     * @param x_union The BST for x_union
     */
    ContourStripes(XInterval x_interval, YInterval y_interval,
                   UnionTree *x_union);
};

/**
 * A function to get the contours of the union of rectangles
 * @return vector of pair of points, where the pair represents 2 ends of a line
 * segment
 */
std::vector<std::pair<Point, Point>>
getContours(std::vector<Rectangle> &rectangles);
