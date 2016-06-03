//
// Created by grey on 03.06.16.
//

#ifndef QUAKE_PLAYER_H
#define QUAKE_PLAYER_H

#include "Object.h"

class Player : public Object {
public:
    Player(int startX, int startY) {
        x = startX;
        y = startY;
    }

    void startThread() {
        std::thread t([this]() { this->startLisening(); });
    }

    void startLisening() {
        while(true) {

        }
    }
};

#endif //QUAKE_PLAYER_H
