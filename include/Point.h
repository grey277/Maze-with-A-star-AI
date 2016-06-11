#ifndef POINT_H
#define POINT_H

class Point {
public:
    int x;
    int y;

    Point() : x(1), y(1) { }

    Point(int x, int y) : x(x), y(y) { }

    Point(const Point &p) : x(p.x), y(p.y) { }

    Point& operator=(const Point& p) {
        x = p.x;
        y = p.y;
        return *this;
    }

    bool operator==(Point p) {
        return x == p.x && y == p.y;
    }
};

#endif //POINT_H
