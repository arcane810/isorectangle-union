/** @file */

#include "measure.hpp"
#include <algorithm>
#include <float.h>
#include <iostream>
#include <map>
#include <set>

MeasureStripes::MeasureStripes(XInterval x_interval, YInterval y_interval,
                               long double x_union)
    : x_interval(x_interval), y_interval(y_interval), x_union(x_union) {}

/**
 * Copies the stripe and split into partitions as per P
 * @param s set of stripes
 * @param P set of partition borders
 * @param x_int x interval of the strip
 * @return set of new strips
 */
std::vector<MeasureStripes> copy_stripes(std::vector<MeasureStripes> &s,
                                         std::vector<long double> &P,
                                         XInterval x_int) {
    std::vector<MeasureStripes> s_copy;
    int i = 1;
    for (MeasureStripes &stripe : s) {
        while (i < P.size() && stripe.y_interval.top >= P[i]) {
            s_copy.push_back(MeasureStripes(x_int, YInterval(P[i - 1], P[i]),
                                            stripe.x_union));
            i++;
        }
    }
    return s_copy;
}

/**
 * Adds the entire interval to x_union for unmerged edges tha cross entire
 * interval
 * @param s set of stripes
 * @param J set of y ptojection of edges
 */
void blacken(std::vector<MeasureStripes> &s,
             std::vector<std::pair<YInterval, int>> &J) {
    int i = 0;
    for (auto &it : s) {
        while (i < J.size()) {
            if (it.y_interval.top <= J[i].first.top &&
                it.y_interval.bottom >= J[i].first.bottom) {
                it.x_union = it.x_interval.right - it.x_interval.left;
                break;
            } else {
                if (it.y_interval.top <= J[i].first.top) {
                    break;
                } else {
                    i++;
                }
            }
        }
    }
}

/**
 * Concatenates the left and right strips after they are copied and blackened
 * @param s_left set of stripes of left part
 * @param s_right set of stripes of right part
 * @param P set of partition borders
 * @param x_ext the x interval of the combined stripes
 * @return set of concatenated strips
 */
std::vector<MeasureStripes> concatenate(std::vector<MeasureStripes> &s_left,
                                        std::vector<MeasureStripes> &s_right,
                                        std::vector<long double> &P,
                                        XInterval x_ext) {
    std::vector<MeasureStripes> s;
    int n = s_left.size();
    for (int i = 0; i < n; i++) {
        long double new_x_union;
        new_x_union = s_left[i].x_union + s_right[i].x_union;
        s.push_back(MeasureStripes(x_ext, s_left[i].y_interval, new_x_union));
    }
    return s;
}

/**
 * Recursive divide and conquer call to get the set of stripes
 * @param edges set of vertical edges of the rectangle in the interval x_ext
 * @param x_ext the x interval
 * @param L set of y projections of unmerged left edges to which the merged left
 * edges are added
 * @param R set of y projections of unmerged right edges to which the merged
 * right edges are added
 * @param P set of partition borders to which new partition borders are to be
 * added
 * @param s set of stripes to which stripes are to be added
 */
void stripes(std::vector<std::pair<Edge, int>> edges, XInterval x_ext,
             std::vector<std::pair<YInterval, int>> &L,
             std::vector<std::pair<YInterval, int>> &R,
             std::vector<long double> &P, std::vector<MeasureStripes> &s) {
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
            s.push_back(MeasureStripes(x_ext, YInterval(P[i - 1], P[i]), 0));
        }
        if (edges[0].first.edge_type == LEFT) {
            s[1].x_union = x_ext.right - edges[0].first.x_coordinate;
        } else {
            s[1].x_union = edges[0].first.x_coordinate - x_ext.left;
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
        std::vector<MeasureStripes> s_left;
        std::vector<std::pair<YInterval, int>> L_right;
        std::vector<std::pair<YInterval, int>> R_right;
        std::vector<long double> P_right;
        std::vector<MeasureStripes> s_right;

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
        int i = 0, j = 0;
        while (i < L_left.size() || j < L_right.size()) {
            if (i < L_left.size() && cnt[L_left[i].second] >= 2) {
                i++;
                continue;
            }
            if (i >= L_left.size()) {
                L.push_back(L_right[j++]);
            } else if (j >= L_right.size()) {
                L.push_back(L_left[i++]);
            } else if (L_left[i].first.bottom < L_right[j].first.bottom) {
                L.push_back(L_left[i++]);
            } else {
                L.push_back(L_right[j++]);
            }
        }

        // R Union
        i = 0, j = 0;
        while (i < R_right.size() || j < R_left.size()) {
            if (i < R_right.size() && cnt[R_right[i].second] >= 2) {
                i++;
                continue;
            }
            if (i >= R_right.size()) {
                R.push_back(R_left[j++]);
            } else if (j >= R_left.size()) {
                R.push_back(R_right[i++]);
            } else if (R_right[i].first.bottom < R_left[j].first.bottom) {
                R.push_back(R_right[i++]);
            } else {
                R.push_back(R_left[j++]);
            }
        }

        // P Union
        std::set<long double> P_set;
        for (long double i : P_left)
            P_set.insert(i);
        for (long double i : P_right)
            P_set.insert(i);
        for (long double i : P_set)
            P.push_back(i);

        std::vector<MeasureStripes> s_left2 =
            copy_stripes(s_left, P, XInterval(x_ext.left, div));
        std::vector<MeasureStripes> s_right2 =
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

long double getMeasure(std::vector<Rectangle> rectangles) {
    std::vector<std::pair<YInterval, int>> L;
    std::vector<std::pair<YInterval, int>> R;
    std::vector<long double> P;
    std::vector<MeasureStripes> s;
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
    long double ans = 0;
    for (auto stripe : s) {
        ans += (stripe.y_interval.top - stripe.y_interval.bottom) *
               (stripe.x_union);
    }
    return ans;
}