//
// Created by grey on 31.05.16.
//

#ifndef QUAKEWITHSOCKETS_GAME_H
#define QUAKEWITHSOCKETS_GAME_H

class Object {
private:
    int x;
    int y;
    enum type { WALL = 0, PLAYER = 1, ENEMY = 2, BOT = 3 };
    virtual Object(int x, int y) : x(x), y(y) { }
    virtual Object() : x(0), y(0) { }
};


#endif //QUAKEWITHSOCKETS_GAME_H
