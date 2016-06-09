//
// Created by grey on 03.06.16.
//

#ifndef QUAKE_MAP_H
#define QUAKE_MAP_H

#include <time.h>
#include <cstdlib>

#include <string.h>
#include <mutex>

#include "MapGen.h"

enum objectType {
    WALL = 0, PLAYER = 1, ENEMY = 2, NOTHING = 4, ITEM = 5, ROUTE = 6, SHOOT = 7
};

class Map {
private:
    const int horizontalSize;
    const int verticalSize;
    int **map;
    int playerX, playerY;
    static std::mutex lock;

    class LockMutex
    {
    public:

        inline LockMutex()
        {
            lock.lock();
        };

        inline ~LockMutex()
        {
            lock.unlock();
        };
    };

public:
    Map(int horizontalSize, int verticalSize)
            : horizontalSize(horizontalSize), verticalSize(verticalSize), playerX(2), playerY(2) {
        MapGen gen(horizontalSize, verticalSize);
        map = gen.getMaze();

    }

    const char* toCharStr() {
        LockMutex lock;
        std::string m = "";
        for (int i = 0; i < horizontalSize; ++i) {
            for (int j = 0; j < verticalSize; ++j) {
                m += (char)((int)map[i][j] + 48);
            }
        }
        return m.c_str();
    }

    void changeMap(const char* m, size_t size){
        LockMutex lock;
        std::string message(m);
        int pos = 0;
        for (int x = 0; x < horizontalSize; ++x) {
            for (int y = 0; y < verticalSize; ++y) {
                switch(message.at(pos)){
                    case '0': map[x][y] = WALL; break;
                    case '1': map[x][y] = PLAYER;
                        playerX = x;
                        playerY = y;
                        break;
                    case '2': map[x][y] = ENEMY; break;
                    case '4': map[x][y] = NOTHING; break;
                    case '5': map[x][y] = ITEM; break;
                    case '6': map[x][y] = ROUTE; break;
                    case '7': map[x][y] = SHOOT; break;
                    default: break;
                }
                pos++;
            }
        }
    }

    int getHorizontalSize(){ return horizontalSize; }

    int getVerticalSize() { return verticalSize; }

    int getPlayerX() {
        LockMutex lock;
        return playerX; }

    int getPlayerY() {
        LockMutex lock;
        return playerY; }

    bool canMove(int x, int y) {
        LockMutex lock;
        return !(x < 0 || x >= horizontalSize || y < 0 || y >= verticalSize
                 || map[x][y] == WALL || map[x][y] == PLAYER || map[x][y] == ENEMY );
    }

    void addBullet(int x, int y) {
        map[x][y] = SHOOT;
    }

    bool stopBullet(int x, int y) {
        return map[x][y] == WALL || map[x][y] == PLAYER || map[x][y] == ENEMY;
    }

    bool canShoot(int botPosX, int botPosY, int playerPosX, int playerPosY){
        LockMutex lock;
        if(botPosX == playerPosX) {
            if(botPosY < playerPosY) {
                for (int y = botPosY + 1; y < playerPosY; ++y) {
                    if(map[botPosX][y] == WALL){
                        return false;
                    }
                }
                return true;
            }
            for (int y = botPosY - 1; y < playerPosY; --y) {
                if(map[botPosX][y] == WALL){
                    return false;
                }
            }
            return true;
        }
        else if(botPosY == playerPosY){
            if(botPosX < playerPosX) {
                for (int x = botPosX + 1; x < playerPosX; ++x) {
                    if(map[botPosY][x] == WALL){
                        return false;
                    }
                }
                return true;
            }
            for (int x = botPosX - 1; x < playerPosX; --x) {
                if(map[botPosY][x] == WALL){
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    void setPlayerPosition(int x, int y) {
        LockMutex lock;
        map[x][y] = PLAYER;
        playerX= x;
        playerY = y;
    }

    void setBotPosition(int x, int y) {
        LockMutex lock;
        map[x][y] = ENEMY;
    }

    void updateBotPosition(int oldX, int oldY, int x, int y) {
        LockMutex lock;
        map[oldX][oldY] = NOTHING;
        map[x][y] = ENEMY;
    }

    void updatePlayerPosition(int oldX, int oldY, int x, int y) {
        LockMutex lock;
        map[oldX][oldY] = NOTHING;
        map[x][y] = PLAYER;
        playerX = x;
        playerY = y;
    }

    const int operator()(const int x, const int y) const {
        LockMutex lock;
        return map[x][y];
    }

    int** getMap(){
        LockMutex lock;
        return map;
    }
};

std::mutex Map::lock;

#endif //QUAKE_MAP_H
