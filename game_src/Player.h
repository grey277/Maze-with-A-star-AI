//
// Created by grey on 03.06.16.
//

#ifndef QUAKE_PLAYER_H
#define QUAKE_PLAYER_H

#include "Object.h"

class Player : public Object {
private:
    Game* _game;
public:
    Player(int startX, int startY, Game* game) : _game(game) {
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
                _game->move(this, ch);
            }
        }
    }
};

#endif //QUAKE_PLAYER_H
