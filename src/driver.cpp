#include "contours.hpp"
#include <fstream>
#include <iostream>

int main() {
    std::vector<Rectangle> rectangles;
    std::ifstream fin("test1.in");
    int n;
    fin >> n;
    for (int i = 0; i < n; i++) {
        int a, b, c, d;
        fin >> a >> b >> c >> d;
        rectangles.push_back(Rectangle(XInterval(a, b), YInterval(c, d)));
    }
    getContours(rectangles);

    return 0;
}