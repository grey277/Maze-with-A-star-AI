//
// Created by raiden on 04.06.16.
//

#ifndef QUAKE_BOT_H
#define QUAKE_BOT_H

#include "../game_include/findShortestPath.h"

class Bot : protected Object {
private:
    server *_server;
    Map *_map;
    Renderer *_renderer;
public:
    Bot(int startX, int startY, client *client, Map *map, Renderer *renderer) : _client(client), _map(map),
                                                                                _renderer(renderer) {
        x = startX;
        y = startY;
        _map->setBotPosition(x, y);
        std::thread t([this]() { this->startThread(); });
    }

    void startThread() {

    }

    int getX() { return x; }

    int getY() { return y; }
};


#endif //QUAKE_BOT_H
