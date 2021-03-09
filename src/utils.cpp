/** @file */

#include "utils.hpp"
#include <algorithm>
#include <utility>

Point::Point(long double x, long double y) : y(y), x(x) {}

Point::Point() : y(0), x(0) {}

XInterval::XInterval(long double left, long double right) {
    if (left > right)
        std::swap(left, right);
    this->left = left;
    this->right = right;
}
XInterval::XInterval() : left(0), right(0) {}

YInterval::YInterval(long double bottom, long double top) {
    if (bottom > top)
        std::swap(bottom, top);
    this->bottom = bottom;
    this->top = top;
}
YInterval::YInterval() : bottom(0), top(0) {}

Edge::Edge(YInterval y_interval, long double x_coordinate, EdgeType edge_type)
    : y_interval(y_interval), x_coordinate(x_coordinate), edge_type(edge_type) {
}

Edge::Edge() : y_interval(YInterval()), x_coordinate(0) {}

Rectangle::Rectangle(XInterval x_interval, YInterval y_interval)
    : x_interval(x_interval), y_interval(y_interval) {}

Rectangle::Rectangle(long double x_left, long double x_right,
                     long double y_bottom, long double y_top)
    : x_interval(XInterval(x_left, x_right)),
      y_interval(YInterval(y_bottom, y_top)) {}

Rectangle::Rectangle(Point p1, Point p2)
    : Rectangle(std::min(p1.x, p2.x), std::max(p1.x, p2.x),
                std::min(p1.y, p2.y), std::max(p1.y, p2.y)) {}