/** @file */

#pragma once

/**
 * A Class for a point in 2-D Space
 */
class Point {
  public:
    /// The x coordinate of the point
    long double x;
    /// The y coordinate of the point
    long double y;

    /**
     * Constructor. Initializes the coordinates to the passed parameters
     *
     * @param x The x coordinate
     * @param y The y coordinate
     */
    Point(long double x, long double y);

    /**
     * Default Constructor
     */
    Point();
};

/**
 * A class representing a horizontal interval
 */
class XInterval {
  public:
    /// The left x coordinate of the interval
    long double left;
    /// The right x coordinate of the interval
    long double right;

    /**
     * Constructor. Initializes the limits of the intervals to the passed
     * parameters
     *
     * @param left The left limit
     * @param right The right limit
     */
    XInterval(long double left, long double right);

    /**
     * Default Constructor
     */
    XInterval();
};

/**
 * A class representing a vertical interval
 */
class YInterval {
  public:
    /// The lower y coordinate of the interval
    long double bottom;
    /// The upper y coordinate of the interval
    long double top;

    /**
     * Constructor. Initializes the limits of the intervals to the passed
     * parameters
     *
     * @param bottom The bottom limit
     * @param top The top limit
     */
    YInterval(long double bottom, long double top);

    /**
     * Default Constructor
     */
    YInterval();
};

/**
 * An enum for the type of the edge of the rectangle
 */
enum EdgeType { LEFT, RIGHT, TOP, BOTTOM };

/**
 * A Class which stores information for a vertical line segment which can act as
 * an edge of a rectangle
 */
class Edge {
  public:
    /// The vertical interval in which the segment lies
    YInterval y_interval;
    /// The x coordinate at which the segment lies
    long double x_coordinate;
    EdgeType edge_type;
    /**
     * Constructor. Initialize the information to the passed parameters
     *
     * @param y_interval vertical interval of the segment
     * @param x_coordinate x coordinate of the segment
     */
    Edge(YInterval y_interval, long double x_coordinate, EdgeType edge_type);

    /**
     * Default Constructor
     */
    Edge();
};

/**
 * A Class which stores information for a horizontal line segment which can act
 * as an edge of a rectangle
 */
class HorizontalEdge {
  public:
    /// The vertical interval in which the segment lies
    XInterval x_interval;
    /// The x coordinate at which the segment lies
    long double y_coordinate;
    EdgeType edge_type;
    /**
     * Constructor. Initialize the information to the passed parameters
     *
     * @param y_interval vertical interval of the segment
     * @param x_coordinate x coordinate of the segment
     */
    HorizontalEdge(XInterval x_interval, long double y_coordinate,
                   EdgeType edge_type);

    /**
     * Default Constructor
     */
    HorizontalEdge();
};

/**
 * A Rectangle class
 */
class Rectangle {
  public:
    /// The left and right limits of the rectangle
    XInterval x_interval;
    /// The bottom and top limits of the rectangle
    YInterval y_interval;

    /**
     * Constructor. Initialize the limits with intervals as input
     *
     * @param x_interval horizontal limits of the rectangle
     * @param y_interval vertical limits of the rectangle
     */
    Rectangle(XInterval x_interval, YInterval y_interval);

    /**
     * Constructor. Initialize the limits with coordinates
     *
     * @param x_left left limit of the rectangle
     * @param x_right right limit of the rectangle
     * @param y_bototm bottom limit of the rectangle
     * @param y_top top limit of the rectangle
     */
    Rectangle(long double x_left, long double x_right, long double y_bottom,
              long double y_top);

    /**
     * Constructor. Initialize the limits with 2 opposite vertices as input
     *
     * @param p1 A vertex of the rectangle
     * @param p2 Another vertex opposite to p1
     */
    Rectangle(Point p1, Point p2);
};
