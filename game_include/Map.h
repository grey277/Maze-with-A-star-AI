//
// Created by grey on 03.06.16.
//

#ifndef QUAKE_MAP_H
#define QUAKE_MAP_H

#include <time.h>
#include <cstdlib>

class Map {
private:
    const int horizontalSize;
    const int verticalSize;
    objectType** map;

public:
    Map(int horizontalSize, int verticalSize)
            : horizontalSize(horizontalSize), verticalSize(verticalSize) {
        map = new objectType*[horizontalSize];
        for (int k = 0; k < horizontalSize; ++k) {
            map[k] = new objectType[verticalSize];
        }

        for (int i = 0; i < horizontalSize; i++) {
            for (int j = 0; j < verticalSize; ++j) {
                map[i][j] = NOTHING;
            }
        }
    }

    int getHorizontalSize(){ return horizontalSize; }

    int getVerticalSize() { return verticalSize; }

    bool canMove(int x, int y) {
        return true;
    }

    objectType operator()(int x, int y) {
            return map[x][y];
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
