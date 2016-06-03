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
        startThread();
    }

    void startThread() {
        WINDOW *w = initscr();
        raw();
        cbreak();
        noecho();
        nodelay(w, TRUE);
        keypad(stdscr, TRUE);
        curs_set(0);
        while(true) {
            char ch;
            ch = getch();
            if(ch > 0) {
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
