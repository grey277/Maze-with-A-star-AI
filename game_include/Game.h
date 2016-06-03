//
// Created by grey on 31.05.16.
//

#ifndef QUAKEWITHSOCKETS_GAME_H
#define QUAKEWITHSOCKETS_GAME_H

#include "findShortestPath.h"
#include "Map.h"

using namespace std;

class Game : public Object {
public:
    Game() : _map(30, 30, 20, 20, 1, 1) { }

    void move(Player* p, char direction) {
        switch(direction) {
            case 'a': if(_map((p->x - 1), p->y)) break;
            default: break;
        }
    }

    Map getMap() { return _map; }

private:
    Map _map;
};
#endif //QUAKEWITHSOCKETS_GAME_H
