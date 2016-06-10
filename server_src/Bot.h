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
    server *_server;
    Map *_map;
    int x;
    int y;

    std::list<Point *> *path;

public:
    Bot(int startX, int startY, server *server, Map *map) : _server(server), _map(map) {
        x = startX;
        y = startY;
        _map->setBotPosition(x, y);

        FindShortestPath f(_map);
        f.pathFind(Point(x, y), Point(_map->getPlayerX(), _map->getPlayerY()));
        path = f.makePatch(Point(x, y));

        path->resize(path->size() / 2);

        _map->setDiamond(f.getMiddle().x, f.getMiddle().y);

        while (!path->empty()) {
            boost::thread t(boost::bind(&Bot::startThread, this));
            t.join();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

    }

    bool startThread() {
        Point *p = path->front();
        path->pop_front();
        if (_map->canMove(p->x, p->y)) {
            _map->updateBotPosition(x, y, p->x, p->y);
            x = p->x;
            y = p->y;
        }
        message msg;
        msg.body_length(std::strlen(_map->toCharStr()));
        std::memcpy(msg.body(), _map->toCharStr(), msg.body_length());
        msg.messageType(message::type::mapm);
        msg.encode_header();
        (*_server).getRoom()->deliver(msg);
        return true;
    }

    int getX() { return x; }

    int getY() { return y; }
};


#endif //QUAKE_BOT_H
