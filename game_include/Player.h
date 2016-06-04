//
// Created by grey on 03.06.16.
//

#ifndef QUAKE_PLAYER_H
#define QUAKE_PLAYER_H

#include <ncurses.h>
#include "Object.h"
#include "../client_src/client.h"
#include "Renderer.h"
#include <iostream>
#include <thread>

using namespace std;

class Player : protected Object {
private:
    client* _client;
    Map* _map;
    Renderer* _renderer;
public:
    Player(int startX, int startY, client* client, Map* map, Renderer* renderer) : _client(client), _map(map),
        _renderer(renderer) {
        x = startX;
        y = startY;
        _map->setPlayerPosition(x, y);
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
                    case 'a': if(_map->canMove(x - 1, y)){
                            send = true;
                            _map->updatePlayerPosition(x, y, x - 1, y);
                            x--;
                        }
                        break;
                    case 'd': if(_map->canMove(x + 1, y)){
                            send = true;
                            _map->updatePlayerPosition(x, y, x + 1, y);
                            x++;
                        }
                        break;
                    case 'w': if(_map->canMove(x, y - 1)){
                            send = true;
                            _map->updatePlayerPosition(x, y, x, y - 1);
                            y--;
                        }
                        break;
                    case 's': if(_map->canMove(x, y + 1)){
                            send = true;
                            _map->updatePlayerPosition(x, y, x, y + 1);
                            y++;
                        }
                        break;
                    case 'q': if(_map->canMove(x - 1, y - 1)){
                            send = true;
                            _map->updatePlayerPosition(x, y, x - 1, y - 1);
                            y--;
                            x--;
                        }
                        break;
                    case 'e': if(_map->canMove(x +1, y - 1)){
                            send = true;
                            _map->updatePlayerPosition(x, y, x + 1, y - 1);
                            y--;
                            x++;
                        }
                        break;
                    case 'c': if(_map->canMove(x + 1, y + 1)){
                            send = true;
                            _map->updatePlayerPosition(x, y, x + 1, y + 1);
                            y++;
                            x++;
                        }
                        break;
                    case 'z': if(_map->canMove(x - 1, y + 1)){
                            send = true;
                            _map->updatePlayerPosition(x, y, x - 1, y + 1);
                            y++;
                            x--;
                        }
                        break;
                    default: break;
                }
                if(send) {
                    message msg;
                    string m = to_string(x) + "," + to_string(y);
                    msg.body_length(std::strlen(m.c_str()));
                    std::memcpy(msg.body(), m.c_str(), msg.body_length());
                    msg.encode_header();
                    _client->write(msg);
                    _renderer->render(_map);
                }
            }
        }
    }

    int getX() { return x; }

    int getY() { return y; }
};

#endif //QUAKE_PLAYER_H
