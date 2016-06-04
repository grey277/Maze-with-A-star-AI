//
// Created by raiden on 04.06.16.
//

#ifndef QUAKE_BOT_H
#define QUAKE_BOT_H

#include "../game_include/findShortestPath.h"
#include "../game_include/Map.h"
#include "server.h"

class Bot : protected Object {
private:
    server *_server;
    Map *_map;
public:
    Bot(int startX, int startY, server *server, Map *map) : _server(server), _map(map), _renderer(renderer) {
        x = startX;
        y = startY;
        _map->setBotPosition(x, y);
        std::thread t([this]() { this->startThread(); });
    }

    void startThread() {
        FindShortestPath f(_map);
        Point p = f.pathFind(new Point(startX, startY), new Point(_map->getPlayerX(), _map->getPlayerY()));
    }

    int getX() { return x; }

    int getY() { return y; }
};


#endif //QUAKE_BOT_H
