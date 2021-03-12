/** @file */

#include "contours.hpp"
#include <algorithm>
#include <float.h>
#include <iostream>
#include <map>
#include <set>

ContourStripes::ContourStripes(XInterval x_interval, YInterval y_interval,
                               std::vector<XInterval> x_union)
    : x_interval(x_interval), y_interval(y_interval), x_union(x_union) {}

std::vector<ContourStripes> copy_stripes(std::vector<ContourStripes> &s,
                                         std::vector<long double> &P,
                                         XInterval x_int) {
    std::vector<ContourStripes> s_copy;
    int i = 1;
    for (ContourStripes &stripe : s) {
        while (i < P.size() && stripe.y_interval.top >= P[i]) {
            s_copy.push_back(ContourStripes(x_int, YInterval(P[i - 1], P[i]),
                                            stripe.x_union));
            i++;
        }
    }
    return s_copy;
}

void blacken(std::vector<ContourStripes> &s,
             std::vector<std::pair<YInterval, int>> &J) {
    for (auto &it : s) {
        for (auto ed : J) {
            if (it.y_interval.bottom >= ed.first.bottom &&
                it.y_interval.top <= ed.first.top) {
                it.x_union = std::vector<XInterval>(1, it.x_interval);
                break;
            }
        }
    }
}
std::vector<ContourStripes> concatenate(std::vector<ContourStripes> &s_left,
                                        std::vector<ContourStripes> &s_right,
                                        std::vector<long double> &P,
                                        XInterval x_ext) {
    std::vector<ContourStripes> s;
    int n = s_left.size();
    for (int i = 0; i < n; i++) {
        int n1 = s_left[i].x_union.size();
        int n2 = s_right[i].x_union.size();
        std::vector<XInterval> new_x_union;
        for (int j = 0; j < n1 - 1; j++) {
            new_x_union.push_back(s_left[i].x_union[j]);
        }
        int rrst = 1;
        if (s_left[i].x_union.size() == 0)
            rrst = 0;
        else if (s_right[i].x_union.size() == 0) {
            new_x_union.push_back(s_left[i].x_union.back());
        } else if (s_left[i].x_union.back().right ==
                   s_right[i].x_union[0].left) {
            new_x_union.push_back(XInterval(s_left[i].x_union.back().left,
                                            s_right[i].x_union[0].right));
        } else {
            new_x_union.push_back(s_left[i].x_union.back());
            new_x_union.push_back(s_right[i].x_union[0]);
        }
        for (int j = 1; j < n2; j++) {
            new_x_union.push_back(s_right[i].x_union[j]);
        }
        // std::cout << "NUSZ: " << new_x_union.size() << "\n";
        s.push_back(ContourStripes(x_ext, s_left[i].y_interval, new_x_union));
    }
    return s;
}

void stripes(std::vector<std::pair<Edge, int>> edges, XInterval x_ext,
             std::vector<std::pair<YInterval, int>> &L,
             std::vector<std::pair<YInterval, int>> &R,
             std::vector<long double> &P, std::vector<ContourStripes> &s) {
    if (edges.size() == 1) {
        if (edges[0].first.edge_type == LEFT) {
            L.push_back({edges[0].first.y_interval, edges[0].second});
        } else {
            R.push_back({edges[0].first.y_interval, edges[0].second});
        }
        P.push_back(-LDBL_MAX);
        P.push_back(edges[0].first.y_interval.bottom);
        P.push_back(edges[0].first.y_interval.top);
        P.push_back(LDBL_MAX);
        for (int i = 1; i < P.size(); i++) {
            s.push_back(ContourStripes(x_ext, YInterval(P[i - 1], P[i]),
                                       std::vector<XInterval>()));
        }
        if (edges[0].first.edge_type == LEFT) {
            s[1].x_union.push_back(
                XInterval(edges[0].first.x_coordinate, x_ext.right));
        } else {
            s[1].x_union.push_back(
                XInterval(x_ext.left, edges[0].first.x_coordinate));
        }
    } else {
        // Divide
        int es = edges.size();
        long double div = (edges[es / 2 - 1].first.x_coordinate +
                           edges[es / 2].first.x_coordinate) /
                          2;
        std::vector<std::pair<Edge, int>> edges_left;
        std::vector<std::pair<Edge, int>> edges_right;
        for (int i = 0; i < es / 2; i++) {
            edges_left.push_back(edges[i]);
        }
        for (int i = es / 2; i < es; i++) {
            edges_right.push_back(edges[i]);
        }
        std::vector<std::pair<YInterval, int>> L_left;
        std::vector<std::pair<YInterval, int>> R_left;
        std::vector<long double> P_left;
        std::vector<ContourStripes> s_left;
        std::vector<std::pair<YInterval, int>> L_right;
        std::vector<std::pair<YInterval, int>> R_right;
        std::vector<long double> P_right;
        std::vector<ContourStripes> s_right;

        // Conquer
        stripes(edges_left, XInterval(x_ext.left, div), L_left, R_left, P_left,
                s_left);
        stripes(edges_right, XInterval(div, x_ext.right), L_right, R_right,
                P_right, s_right);

        // Merge
        std::map<int, int> cnt;
        for (auto it : L_left) {
            cnt[it.second]++;
        }
        for (auto it : R_right) {
            cnt[it.second]++;
        }

        // L Union
        for (auto it : L_left) {
            if (cnt[it.second] < 2) {
                L.push_back(it);
            }
        }
        for (auto it : L_right) {
            L.push_back(it);
        }

        // R Union
        for (auto it : R_right) {
            if (cnt[it.second] < 2) {
                R.push_back(it);
            }
        }
        for (auto it : R_left) {
            R.push_back(it);
        }

        // P Union
        std::set<long double> P_set;
        for (long double i : P_left)
            P_set.insert(i);
        for (long double i : P_right)
            P_set.insert(i);
        for (long double i : P_set)
            P.push_back(i);

        std::vector<ContourStripes> s_left2 =
            copy_stripes(s_left, P, XInterval(x_ext.left, div));
        std::vector<ContourStripes> s_right2 =
            copy_stripes(s_right, P, XInterval(div, x_ext.right));

        std::vector<std::pair<YInterval, int>> R2_LR;
        for (auto it : R_right) {
            if (cnt[it.second] < 2) {
                R2_LR.push_back(it);
            }
        }
        blacken(s_left2, R2_LR);
        std::vector<std::pair<YInterval, int>> L1_LR;
        for (auto it : L_left) {
            if (cnt[it.second] < 2) {
                L1_LR.push_back(it);
            }
        }
        blacken(s_right2, L1_LR);
        s = concatenate(s_left2, s_right2, P, x_ext);
    }
}

std::vector<std::pair<Point, Point>>
getContours(std::vector<Rectangle> rectangles) {
    std::vector<std::pair<YInterval, int>> L;
    std::vector<std::pair<YInterval, int>> R;
    std::vector<long double> P;
    std::vector<ContourStripes> s;
    std::vector<std::pair<Edge, int>> edges;
    for (int i = 0; i < rectangles.size(); i++) {
        Rectangle rectangle = rectangles[i];
        edges.push_back(
            {Edge(rectangle.y_interval, rectangle.x_interval.left, LEFT), i});
        edges.push_back(
            {Edge(rectangle.y_interval, rectangle.x_interval.right, RIGHT), i});
    }
    auto cmp = [](std::pair<Edge, int> e1, std::pair<Edge, int> e2) {
        if (e1.first.x_coordinate == e2.first.x_coordinate) {
            if (e1.first.edge_type == LEFT) {
                return true;
            } else {
                return false;
            }
        } else {
            return e1.first.x_coordinate < e2.first.x_coordinate;
        }
    };
    std::sort(edges.begin(), edges.end(), cmp);
    stripes(edges, XInterval(-LDBL_MAX, LDBL_MAX), L, R, P, s);
    // std::cout << "\n///////////////////////////////\nSTRIPS\n";
    // for (auto stripe : s) {
    //     std::cout << " XINT: " << stripe.x_interval.left << " "
    //               << stripe.x_interval.right << " "
    //               << " YINT: " << stripe.y_interval.bottom << " "
    //               << stripe.y_interval.top << "\n";
    //     for (auto it : stripe.x_union) {
    //         std::cout << it.left << " " << it.right << "\n";
    //     }
    //     std::cout << "========================\n";
    // }
    std::vector<std::pair<Point, Point>> ed;
    return ed;
}