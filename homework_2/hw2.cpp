#include <iostream>
using namespace std;

struct Point {
    float x;
    float y;

    Point() : x(0), y(0) {}
    Point(float x, float y) : x(x), y(y) {}
};

void PopulatePointsTable(Point* points, const int n) {
    points[0].x = 62.0;
    points[0].y = 58.4;

    points[1].x = 57.5;
    points[1].y = 56.0;

    points[2].x = 51.7;
    points[2].y = 56.0;

    points[3].x = 67.9;
    points[3].y = 19.6;

    points[4].x = 57.7;
    points[4].y = 42.1;

    points[5].x = 54.2;
    points[5].y = 29.1;

    points[6].x = 46.0;
    points[6].y = 45.1;

    points[7].x = 34.7;
    points[7].y = 45.1;

    points[8].x = 45.7;
    points[8].y = 25.1;

    points[9].x = 34.7;
    points[9].y = 26.4;

    points[10].x = 28.4;
    points[10].y = 31.7;

    points[11].x = 33.4;
    points[11].y = 60.5;

    points[12].x = 22.9;
    points[12].y = 32.7;

    points[13].x = 21.5;
    points[13].y = 45.8;

    points[14].x = 15.3;
    points[14].y = 37.8;

    points[15].x = 15.1;
    points[15].y = 49.6;

    points[16].x = 9.1;
    points[16].y = 52.8;

    points[17].x = 9.1;
    points[17].y = 40.3;

    points[18].x = 2.7;
    points[18].y = 56.8;

    points[19].x = 2.7;
    points[19].y = 33.1;
}

int main(int argc, char **argv) {
    const int n = 20;
    Point* points = new Point[n];
    PopulatePointsTable(points, n);

    /*// points printing - TEST purpose
    for (int i = 0; i < n; i++)
        cout << i + 1 << ": (" << points[i].x << ", " << points[i].y << ")" << endl;*/

    delete[] points;
}