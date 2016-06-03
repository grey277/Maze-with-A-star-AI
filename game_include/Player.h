//
// Created by grey on 03.06.16.
//

#ifndef QUAKE_PLAYER_H
#define QUAKE_PLAYER_H

#include <ncurses.h>
#include "Object.h"
#include "../client_src/client.h"
#include <iostream>
#include <thread>

using namespace std;

class Player : protected Object {
private:
    client* _client;
public:
    Player(int startX, int startY, client* client) : _client(client) {
        x = startX;
        y = startY;
    }

    void startThread() {
        std::thread t([this]() { this->startLisening(); });
    }

    void startLisening() {
        while(true) {
            int ch;
            ch = getch();
            if(ch) {
                message msg;
                msg.body_length(std::strlen(std::to_string(ch).c_str()));
                std::memcpy(msg.body(), std::to_string(ch).c_str(), msg.body_length());
                msg.encode_header();
                _client->write(msg);
            }
        }
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }
};

#endif //QUAKE_PLAYER_H
