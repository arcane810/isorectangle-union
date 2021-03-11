/** @file */

#include "contours.hpp"
#include <float.h>
#include <map>
#include <set>

ContourStripes::ContourStripes(XInterval x_interval, YInterval y_interval,
                               std::vector<XInterval> x_union)
    : x_interval(x_interval), y_interval(y_interval), x_union(x_union) {}

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
        P.push_back(LDBL_MIN);
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
        stripes(edges_right, XInterval(x_ext.right, div), L_right, R_right,
                P_right, s_right);

        // Merge
        std::map<int, int> cnt;
        for (auto it : L_left) {
            cnt[it.second]++;
        }
        for (auto it : R_right) {
            cnt[it.second]++;
        }

        for (auto it : L_left) {
            if (cnt[it.second] < 2) {
                L.push_back(it);
            }
        }
        for (auto it : L_right) {
            L.push_back(it);
        }

        for (auto it : R_right) {
            if (cnt[it.second] < 2) {
                R.push_back(it);
            }
        }
        for (auto it : R_left) {
            R.push_back(it);
        }
        std::set<long double> P_set;
        for (int i : P_left)
            P_set.insert(i);
        for (int i : P_right)
            P_set.insert(i);
        for (int i : P_set)
            P.push_back(i);
    }
}

std::vector<std::pair<Point, Point>>
getContours(std::vector<Rectangle> rectangles) {}