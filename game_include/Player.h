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
    Map* _map;
public:
    Player(int startX, int startY, client* client, Map* map) : _client(client), _map(map){
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
                bool send = false;
                switch(ch) {
                    case 'a': if(_map->canMove(x - 1, y)) send = true; break;
                    case 'd': if(_map->canMove(x + 1, y)) send = true; break;
                    case 'w': if(_map->canMove(x, y + 1)) send = true; break;
                    case 's': if(_map->canMove(x, y - 1)) send = true; break;
                    default: break;
                }
                if(send) {
                    message msg;
                    string m = to_string(to_string(ch);
                    msg.body_length(std::strlen(m.c_str()));
                    std::memcpy(msg.body(), m.c_str(), msg.body_length());
                    msg.encode_header();
                    _client->write(msg);
                }
            }
        }
    }

    int getX() { return x; }

    int getY() { return y; }
};

#endif //QUAKE_PLAYER_H
