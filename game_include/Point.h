//
// Created by grey on 10.06.16.
//

#ifndef QUAKE_POINT_H
#define QUAKE_POINT_H

class Point {
public:
    int x;
    int y;

    Point() : x(0), y(0) { }

    Point(int x, int y) : x(x), y(y) { }

    Point(const Point &p) : x(p.x), y(p.y) { }

    void operator=(Point p) {
        x = p.x;
        y = p.y;
    }

    bool operator==(Point p) {
        return x == p.x && y == p.y;
    }

    //friend ostream &operator<<(ostream &cout, const Point &p) {
    //    return cout << p.x << "," << p.y;
    //}
};

#endif //QUAKE_POINT_H
