/** @file */

#include "contours.hpp"
#include <algorithm>
#include <float.h>
#include <iostream>
#include <map>
#include <set>

UnionTree::UnionTree(long double coord, NodeEdgeType node_edge_type,
                     UnionTree *left, UnionTree *right)
    : coord(coord), node_edge_type(node_edge_type), left(left), right(right){};

ContourStripes::ContourStripes(XInterval x_interval, YInterval y_interval,
                               UnionTree *x_union)
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
    int i = 0;
    for (auto &it : s) {
        while (i < J.size()) {
            if (it.y_interval.top <= J[i].first.top &&
                it.y_interval.bottom >= J[i].first.bottom) {
                it.x_union = nullptr;
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
std::vector<ContourStripes> concatenate(std::vector<ContourStripes> &s_left,
                                        std::vector<ContourStripes> &s_right,
                                        std::vector<long double> &P,
                                        XInterval x_ext) {
    std::vector<ContourStripes> s;
    int n = s_left.size();
    for (int i = 0; i < n; i++) {
        UnionTree *new_x_union = nullptr;
        if (s_left[i].x_union) {
            if (s_right[i].x_union) {
                new_x_union =
                    new UnionTree(s_left[i].x_interval.right, UNDEF,
                                  s_left[i].x_union, s_right[i].x_union);
            } else {
                new_x_union = s_left[i].x_union;
            }
        } else {
            if (s_right[i].x_union) {
                new_x_union = s_right[i].x_union;
            } else {
            }
        }

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
            s.push_back(
                ContourStripes(x_ext, YInterval(P[i - 1], P[i]), nullptr));
        }
        if (edges[0].first.edge_type == LEFT) {
            s[1].x_union = new UnionTree(edges[0].first.x_coordinate, LEFT_U,
                                         nullptr, nullptr);
        } else {
            s[1].x_union = new UnionTree(edges[0].first.x_coordinate, RIGHT_U,
                                         nullptr, nullptr);
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
        int i1 = 0, i2 = 0;
        while (i1 < P_left.size() || i2 < P_right.size()) {
            if (i1 >= P_left.size()) {
                if (P.size() == 0 || P_right[i2] != P.back()) {
                    P.push_back(P_right[i2]);
                }
                i2++;
            } else if (i2 >= P_right.size()) {
                if (P.size() == 0 || P_left[i1] != P.back()) {
                    P.push_back(P_left[i1]);
                }
                i1++;
            } else if (P_left[i1] < P_right[i2]) {
                if (P.size() == 0 || P_left[i1] != P.back()) {
                    P.push_back(P_left[i1]);
                }
                i1++;
            } else {
                if (P.size() == 0 || P_right[i2] != P.back()) {
                    P.push_back(P_right[i2]);
                }
                i2++;
            }
        }

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

void dfs(UnionTree *ut, int l, int r,
         std::vector<std::pair<long double, NodeEdgeType>> &occ) {
    if (!ut)
        return;
    if (ut->node_edge_type != UNDEF) {
        occ.push_back({ut->coord, ut->node_edge_type});
    }
    if (ut->coord >= l)
        dfs(ut->left, l, r, occ);
    if (ut->coord <= r)
        dfs(ut->right, l, r, occ);
}

// std::vector<std::pair<Point, Point>>
// contour_pieces(HorizontalEdge h, std::vector<ContourStripes> s) {}

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
            if (e1.first.edge_type == LEFT && e2.first.edge_type == RIGHT) {
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
    std::vector<HorizontalEdge> horizontal_edges;
    for (Rectangle rectangle : rectangles) {
        horizontal_edges.push_back(HorizontalEdge(
            rectangle.x_interval, rectangle.y_interval.top, TOP));
        horizontal_edges.push_back(HorizontalEdge(
            rectangle.x_interval, rectangle.y_interval.bottom, BOTTOM));
    }
    auto cmp2 = [](HorizontalEdge e1, HorizontalEdge e2) {
        if (e1.y_coordinate == e2.y_coordinate) {
            if (e1.edge_type == BOTTOM) {
                return true;
            } else {
                return false;
            }
        } else {
            return e1.y_coordinate < e2.y_coordinate;
        }
    };
    sort(horizontal_edges.begin(), horizontal_edges.end(), cmp2);
    int i1 = 0;
    int i2 = 0;

    std::vector<std::pair<Point, Point>> ed;
    for (HorizontalEdge h : horizontal_edges) {
        int seli = 0;
        if (h.edge_type == BOTTOM) {
            while (s[i1].y_interval.top < h.y_coordinate) {
                i1++;
            }
            seli = i1;
        } else {
            while (s[i2].y_interval.bottom < h.y_coordinate) {
                i2++;
            }
            seli = i2;
        }
        long double x_l = h.x_interval.left;
        long double x_r = h.x_interval.right;
        std::vector<std::pair<long double, NodeEdgeType>> occ;
        dfs(s[seli].x_union, x_l, x_r, occ);
        long double p = x_l;
        bool f = 1;
        for (auto it : occ) {
            if (h.y_coordinate == 6)
                std::cout << it.second << ":" << it.first << "\n";
            if (it.second == LEFT_U) {
                f = 0;
                if (p < it.first)
                    ed.push_back({Point(p, h.y_coordinate),
                                  Point(it.first, h.y_coordinate)});
            } else if (it.first >= p) {
                f = 1;
                p = it.first;
            }
        }
        if (p < x_r && f) {
            ed.push_back(
                {Point(p, h.y_coordinate), Point(x_r, h.y_coordinate)});
        }
    }

    return ed;
}