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
        if(_server->getRoom()->isRecent()) {
            _map(_server->getRoom()->returnLastMessage());
        }
        FindShortestPath f(_map);
        Point p = f.pathFind(new Point(startX, startY), new Point(_map->getPlayerX(), _map->getPlayerY()));
        if(_map->canMove(p.getXPos(), p.getYPos())) {
            _map->updateBotPosition(x, y, p.getXPos(), p.getYPos());
            x = p.getXPos();
            y = p.getYPos();
        }
        message msg;
        msg.body_length(std::strlen(_map->toCharStr()));
        std::memcpy(msg.body(), _map->toCharStr(), msg.body_length());
        msg.encode_header();
        _server->getRoom()->deliver(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    int getX() { return x; }

    int getY() { return y; }
};


#endif //QUAKE_BOT_H
