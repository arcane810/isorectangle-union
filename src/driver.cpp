#include "contours.hpp"
#include "measure.hpp"
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std::chrono;
using namespace std;

int main(int argc, char *argv[]) {
    std::vector<Rectangle> rectangles;
    ifstream fin(argv[1]);
    string ofile = string(argv[1]) + ".out";
    ofstream fout(ofile);
    int n;
    fin >> n;
    for (int i = 0; i < n; i++) {
        long double a, b, c, d;
        fin >> a >> b >> c >> d;
        rectangles.push_back(Rectangle(XInterval(a, b), YInterval(c, d)));
    }
    auto start = high_resolution_clock::now();
    long double mes = getMeasure(rectangles);
    cout << setprecision(18);
    fout << setprecision(18);
    cout << mes << "\n";
    fout << mes << "\n";
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time Taken for measure: " << duration.count() / 1000000.0
         << " s\n";
    std::vector<std::pair<Point, Point>> hcontours = getContours(rectangles);
    fout << hcontours.size() << "\n";
    for (auto it : hcontours) {
        fout << it.first.x << " " << it.first.y << " " << it.second.x << " "
             << it.second.y << "\n";
    }
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - stop);
    cout << "Time Taken for contours: " << duration2.count() / 1000000.0
         << " s\n";

    return 0;
}