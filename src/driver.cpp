#include "contours.hpp"
#include "measure.hpp"
#include <fstream>
#include <iostream>

int main() {
    std::vector<Rectangle> rectangles;
    // std::ifstream fin("test1.in");
    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        int a, b, c, d;
        std::cin >> a >> b >> c >> d;
        rectangles.push_back(Rectangle(XInterval(a, b), YInterval(c, d)));
    }
    std::cout << getMeasure(rectangles) << "\n";
    std::vector<std::pair<Point, Point>> hcontours = getContours(rectangles);
    for (auto it : hcontours) {
        std::cout << it.first.x << " " << it.first.y << " " << it.second.x
                  << " " << it.second.y << "\n";
    }

    return 0;
}