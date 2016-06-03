//
// Created by grey on 31.05.16.
//

#ifndef QUAKEWITHSOCKETS_GAME_H
#define QUAKEWITHSOCKETS_GAME_H

#include <time.h>
#include <cstdlib>
#include <iostream>

#include "findShortestPath.h"

using namespace std;

class Object {
public:
    int x;
    int y;

public:
    enum objectType { //todo
        WALL = 0, PLAYER = 1, ENEMY = 2, NOTHING = 4, ITEM = 5, ROUTE = 6
    };


    Object(int x, int y) : x(x), y(y) { }

    Object() : x(0), y(0) { }

    friend class Map;
    friend class FindShortestPath;
    friend class Player;

};

class Map : public Object {
private:
    int botPossX, botPosY, playerPosX, playerPosY;
    const int horizontalSize;
    const int verticalSize;
    objectType** map;

public:
    Map(int horizontalSize, int verticalSize, int botPossX, int botPosY, int playerPosX, int playerPosY)
            : horizontalSize(horizontalSize), verticalSize(verticalSize),
              botPossX(botPossX), botPosY(botPosY), playerPosX(playerPosX), playerPosY(playerPosY)  {
        map = new objectType*[horizontalSize];
        for (int k = 0; k < horizontalSize; ++k) {
            map[k] = new objectType[verticalSize];
        }

        for (int i = 0; i < horizontalSize; i++) {
            for (int j = 0; j < verticalSize; ++j) {
                map[i][j] = NOTHING;
            }
        }

        map[botPossX][botPosY] = ENEMY;
        map[playerPosX][playerPosY] = PLAYER;
    }

    void makeMap(){
        for (int x = 2 + horizontalSize / 8; x < horizontalSize * 7 / 8 - 1; x++) {
            map[x][verticalSize / 2] = WALL;
        }
        for (int y = 1 + verticalSize / 8; y < verticalSize * 7 / 8; y++) {
            map[horizontalSize / 2][y] = WALL;
        }
    }

    void makeRandomMap() {
        srand(time(NULL));

        for (int i = 0; i < horizontalSize; i++) {
            for (int j = 0; j < verticalSize; ++j) {
                int randomize = rand() % 128;
                if (randomize <= 96) // 75% of map is NOTHING and 25% is WALL
                    map[i][j] = NOTHING;
                else
                    map[i][j] = WALL;
            }
        }
        map[botPossX][botPosY] = ENEMY;
        map[playerPosX][playerPosY] = PLAYER;
    }

    void generate() { // updating map, can make new item or nothing
        srand(time(NULL));
        int horizontal, vertical;
        do {
            horizontal = rand() % horizontalSize;
            vertical = rand() % verticalSize;
        }while(map[horizontal][vertical] != NOTHING);

        int randomize = rand() % 128;
        if (randomize <= 124) // 97% chances to make ITEM
            map[horizontal][vertical] = NOTHING;
        else
            map[horizontal][vertical] = ITEM;

    }

    objectType** getMap(){
        return map;
    }
};

class Game : public Object {

};

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


#endif //QUAKEWITHSOCKETS_GAME_H
