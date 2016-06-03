//
// Created by raiden on 03.06.16.
//

#ifndef QUAKE_RENDERER_H
#define QUAKE_RENDERER_H

#include <ncurses.h>

class Renderer {
    const int horizontalSize, verticalSize;

public:
    Renderer(int horizontalSize, int verticalSize) : horizontalSize(horizontalSize), verticalSize(verticalSize) {}

    void printTable(objectType** map) {
        for (int y = 0; y < verticalSize; y++) {
            for (int x = 0; x < horizontalSize; x++) {
                cout << " ";
                switch(map[x][y]){
                    case WALL:
                        cout << "O";
                        break;
                    case PLAYER:
                        cout << "F"; //finish
                        break;
                        //case ENEMY:
                        //    __throw_domain_error("cannot resolve symbol"); // todo
                        //    break;
                    case ENEMY:
                        cout << "S"; //start
                        break;
                    case NOTHING:
                        cout << ".";
                        break;
                    case ITEM:
                        cout << "I";
                        break;
                    default:
                        __throw_domain_error("cannot resolve symbol");
                }
            }
            cout << endl;
        }
    }

    void render(objectType** map){

        initscr(); //initialize screen
        curs_set(0);

        printTable(map);
        refresh();
        endwin();

    }
};


#endif //QUAKE_RENDERER_H
