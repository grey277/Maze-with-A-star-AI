//
// Created by raiden on 04.06.16.
//

#ifndef QUAKE_BOT_H
#define QUAKE_BOT_H

#include <future>
#include <boost/shared_ptr.hpp>

#include "../game_include/findShortestPath.h"
#include "../game_include/Map.h"
#include "../game_include/Object.h"
#include "server.h"

class Bot : protected Object {
private:
    server* _server;
    Map *_map;
public:
    Bot(int startX, int startY, server *server, Map *map) : _server(server), _map(map) {
        x = startX;
        y = startY;
        _map->setBotPosition(x, y);
        //std::thread t([this](Map* _map) -> void { startThread(_map); });
        while(true) {
            auto t = std::async(&Bot::startThread, this, _map, _server);
            auto info = t.get();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

    }

    bool startThread(Map *_map, server *_server) {
        if((*_server).getRoom()->isRecent()) {
            _map->changeMap((*_server).getRoom()->returnLastMessage().body(), (*_server).getRoom()->returnLastMessage().body_length());
        }
        FindShortestPath f(_map);
        Point p = f.pathFind(Point(x, y), Point(_map->getPlayerX(), _map->getPlayerY()));
        if(_map->canMove(p.getXPos(), p.getYPos())) {
            _map->updateBotPosition(x, y, p.getXPos(), p.getYPos());
            x = p.getXPos();
            y = p.getYPos();
        }
        if(_map->canShoot(x, y, p.getXPos(), p.getYPos())){
            cout << "shooting " << endl;
        }
        message msg;
        msg.body_length(std::strlen(_map->toCharStr()));
        std::memcpy(msg.body(), _map->toCharStr(), msg.body_length());
        msg.encode_header();
        (*_server).getRoom()->deliver(msg);
        return true;
    }

    int getX() { return x; }

    int getY() { return y; }
};


#endif //QUAKE_BOT_H
