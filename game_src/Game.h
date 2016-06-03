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

    Map getMap() { return _map; }

private:
    Map _map;
};
#endif //QUAKEWITHSOCKETS_GAME_H
