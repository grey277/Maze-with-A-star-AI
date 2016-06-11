#ifndef MAP_H
#define MAP_H

#include <mutex>
#include "MapGen.h"
#include "Point.h"

enum objectType {
    WALL = 0, PLAYER = 1, ENEMY = 2, NOTHING = 4, DIAMOND = 5
};

class Map {
private:
    const int horizontalSize;
    const int verticalSize;
    int **map;
    Point playerPosition, botPosition;
    Point diamond;
    static std::mutex lock;

    class LockMutex {
    public:

        inline LockMutex() {
            lock.lock();
        };

        inline ~LockMutex() {
            lock.unlock();
        };
    };

public:
    Map(int horizontalSize, int verticalSize)
            : horizontalSize(horizontalSize), verticalSize(verticalSize), playerPosition(2, 2) {
        MapGen gen(horizontalSize, verticalSize);
        map = gen.getMaze();

        setPlayerPosition(2, 2);
    }

    void setDiamond(int x, int y) {
        LockMutex lock;
        map[x][y] = DIAMOND;
        diamond = Point(x, y);
    }

    const char *toCharStr() {
        LockMutex lock;
        std::string m = "";
        for (int i = 0; i < horizontalSize; ++i) {
            for (int j = 0; j < verticalSize; ++j) {
                m += (char) ((int) map[i][j] + 48);
            }
        }
        return m.c_str();
    }

    void changeMap(const char *m, size_t size) {
        LockMutex lock;
        std::string message(m);
        int pos = 0;
        for (int x = 0; x < horizontalSize; ++x) {
            for (int y = 0; y < verticalSize; ++y) {
                switch (message.at(pos)) {
                    case '0':
                        map[x][y] = WALL;
                        break;
                    case '1':
                        map[x][y] = PLAYER;
                        playerPosition = Point(x, y);
                        break;
                    case '2':
                        map[x][y] = ENEMY;
                        botPosition = Point(x, y);
                        break;
                    case '4':
                        map[x][y] = NOTHING;
                        break;
                    case '5':
                        map[x][y] = DIAMOND;
                        break;
                    default:
                        break;
                }
                pos++;
            }
        }
    }

    int getHorizontalSize() { return horizontalSize; }

    int getVerticalSize() { return verticalSize; }

    int getPlayerX() {
        LockMutex lock;
        return playerPosition.x;
    }

    int getPlayerY() {
        LockMutex lock;
        return playerPosition.y;
    }

    bool canMove(int x, int y) {
        LockMutex lock;
        return !(x < 0 || x >= horizontalSize || y < 0 || y >= verticalSize
                 || map[x][y] == WALL || map[x][y] == PLAYER || map[x][y] == ENEMY);
    }

    void setPlayerPosition(int x, int y) {
        LockMutex lock;
        map[x][y] = PLAYER;
        playerPosition = Point(x, y);
    }

    void setBotPosition(int x, int y) {
        LockMutex lock;
        map[x][y] = ENEMY;
        botPosition = Point(x, y);
    }

    void updateBotPosition(int oldX, int oldY, int x, int y) {
        LockMutex lock;
        map[oldX][oldY] = NOTHING;
        map[x][y] = ENEMY;
        botPosition = Point(x, y);
    }

    void updatePlayerPosition(int oldX, int oldY, int x, int y) {
        LockMutex lock;
        map[oldX][oldY] = NOTHING;
        map[x][y] = PLAYER;
        playerPosition = Point(x, y);
    }

    const int operator()(const int x, const int y) const {
        LockMutex lock;
        return map[x][y];
    }

    int **getMap() {
        LockMutex lock;
        return map;
    }

    int didWon() {
        if (map[diamond.x + 1][diamond.y] == PLAYER || map[diamond.x - 1][diamond.y] == PLAYER
            || map[diamond.x][diamond.y + 1] == PLAYER || map[diamond.x][diamond.y - 1] == PLAYER)
            return (int) PLAYER;
        else if (map[diamond.x + 1][diamond.y] == ENEMY || map[diamond.x - 1][diamond.y] == ENEMY
                 || map[diamond.x][diamond.y + 1] == ENEMY || map[diamond.x][diamond.y - 1] == ENEMY)
            return (int) ENEMY;
        return (int) NOTHING;
    }
};

std::mutex Map::lock;

#endif //QUAKE_MAP_H
