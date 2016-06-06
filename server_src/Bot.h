//
// Created by raiden on 04.06.16.
//

#ifndef QUAKE_BOT_H
#define QUAKE_BOT_H

#include <future>
#include <boost/shared_ptr.hpp>

#include "../game_include/findShortestPath.h"
#include "../game_include/Map.h"
#include "server.h"

class Bot {
private:
    server* _server;
    Map *_map;
    int x;
    int y;
public:
    Bot(int startX, int startY, server *server, Map *map) : _server(server), _map(map) {
        x = startX;
        y = startY;
        _map->setBotPosition(x, y);
        while(true) {
            auto t = std::async(&Bot::startThread, this, _map, _server);
            auto info = t.get();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

    }

    bool startThread(Map *_map, server *_server) {
        FindShortestPath f(_map);
        Point p = f.pathFind(Point(x, y), Point(_map->getPlayerX(), _map->getPlayerY()));
        if(_map->canMove(p.getXPos(), p.getYPos())) {
            _map->updateBotPosition(x, y, p.getXPos(), p.getYPos());
            x = p.getXPos();
            y = p.getYPos();
        }
        if(_map->canShoot(x, y, p.getXPos(), p.getYPos())){
            startShoot(x, y);
        }
        message msg;
        msg.body_length(std::strlen(_map->toCharStr()));
        std::memcpy(msg.body(), _map->toCharStr(), msg.body_length());
        msg.encode_header();
        (*_server).getRoom()->deliver(msg);
        return true;
    }

    void startShoot(int x, int y) {
        auto t = std::async(&Bot::shoot, this,x, y, _map, _server);
        auto info = t.get();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    bool shoot(int x, int y, Map *_map, server *_server) {
        while(!_map->stopBullet(x, y)) {
            message msg;
            std::string s = "shoot" + std::to_string(x) + "," + std::to_string(y);
            msg.body_length(s.length());
            std::memcpy(msg.body(), s.c_str(), msg.body_length());
            msg.encode_header();
            (*_server).getRoom()->deliver(msg);
            x++;
        }
        return true;
    }

    int getX() { return x; }

    int getY() { return y; }
};


#endif //QUAKE_BOT_H
