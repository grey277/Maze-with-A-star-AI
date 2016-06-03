//
// Created by grey on 03.06.16.
//

#ifndef QUAKE_OBJECT_H
#define QUAKE_OBJECT_H

class Object {
protected:
    int x;
    int y;

public:
    enum objectType {
        WALL = 0, PLAYER = 1, ENEMY = 2, NOTHING = 4, ITEM = 5, ROUTE = 6
    };


    Object(int x, int y) : x(x), y(y) { }

    Object() : x(0), y(0) { }

    friend class Map;
    friend class FindShortestPath;
    friend class Player;

};

#endif //QUAKE_OBJECT_H
