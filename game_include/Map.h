//
// Created by grey on 03.06.16.
//

#ifndef QUAKE_MAP_H
#define QUAKE_MAP_H

#include <time.h>
#include <cstdlib>

#include <string.h>

enum objectType {
    WALL = 0, PLAYER = 1, ENEMY = 2, NOTHING = 4, ITEM = 5, ROUTE = 6
};

class Map {
private:
    const int horizontalSize;
    const int verticalSize;
    objectType** map;

    int playerX, playerY;

public:
    Map(int horizontalSize, int verticalSize)
            : horizontalSize(horizontalSize), verticalSize(verticalSize), playerX(1), playerY(1) {
        map = new objectType*[horizontalSize];
        for (int k = 0; k < horizontalSize; ++k) {
            map[k] = new objectType[verticalSize];
        }

        for (int i = 0; i < horizontalSize; i++) {
            for (int j = 0; j < verticalSize; ++j) {
                map[i][j] = NOTHING;
            }
        }
        makeMap();
    }

    char* toCharStr() {
        std::string m = "";
        for (int i = 0; i < horizontalSize; ++i) {
            for (int j = 0; j < verticalSize; ++j) {
                m += (char)((int)map[i][j]);
            }
        }
        return const_cast<char*>(m.c_str());
    }

    objectType** toObjectTypeArray(char* m){
        int pos = 0;
        for (int y = 0; y < mapPointer->getVerticalSize(); y++) {
            for (int x = 0; x < mapPointer->getHorizontalSize(); x++) {
                switch(m[pos]){
                    case WALL:
                        mvaddch(y,x,'O');
                        break;
                    case PLAYER:
                        mvaddch(y,x,'F');
                        break;
                        //case ENEMY:
                        //    __throw_domain_error("cannot resolve symbol"); // todo
                        //    break;
                    case ENEMY:
                        mvaddch(y,x,'S');
                        break;
                    case NOTHING:
                        mvaddch(y,x,'.');
                        break;
                    case ITEM:
                        mvaddch(y,x,'I');
                        break;
                    default:
                        __throw_domain_error("cannot resolve symbol");
                }
                pos++;
            }
        }
    }

    int getHorizontalSize(){ return horizontalSize; }

    int getVerticalSize() { return verticalSize; }

    int getPlayerX() { return playerX; }

    int getPlayerY() { return playerY; }

    bool canMove(int x, int y) {
        return !(x < 0 || x >= horizontalSize || y < 0 || y >= verticalSize || map[x][y] == WALL || map[x][y] == PLAYER || map[x][y] == ENEMY );
    }

    void setPlayerPosition(int x, int y) {
        map[x][y] = PLAYER;
        playerX= x;
        playerY = y;
    }

    void setBotPosition(int x, int y) {
        map[x][y] = ENEMY;
    }

    void updatePlayerPosition(int oldX, int oldY, int x, int y) {
        map[oldX][oldY] = NOTHING;
        map[x][y] = PLAYER;
        playerX = x;
        playerY = y;
    }

    const objectType operator()(const int x, const int y) const {
            return map[x][y];
    }

    void makeMap(){
        for (int x = 2 + horizontalSize / 8; x < horizontalSize * 7 / 8 - 1; x++) {
            map[x][verticalSize / 2] = WALL;
        }
        for (int y = 1 + verticalSize / 8; y < verticalSize * 7 / 8; y++) {
            map[horizontalSize / 2][y] = WALL;
        }

        for(int x = 0; x < horizontalSize; x++){
            map[x][0] = map[x][verticalSize - 1] = WALL;
        }

        for(int y = 0; y < verticalSize; y++){
            map[0][y] = map[horizontalSize - 1][y] = WALL;
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

#endif //QUAKE_MAP_H
