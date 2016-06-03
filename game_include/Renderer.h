//
// Created by raiden on 03.06.16.
//

#ifndef QUAKE_RENDERER_H
#define QUAKE_RENDERER_H

#include <ncurses.h>

class Renderer {
public:
    void printTable(Map* mapPointer) {
        for (int y = 0; y < mapPointer->getVerticalSize(); y++) {
            for (int x = 0; x < mapPointer->getHorizontalSize(); x++) {
                switch((*mapPointer)(x,y)){
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
            }
        }
    }

    void render(Map* map){
        printTable(map);
        refresh();
    }
};


#endif //QUAKE_RENDERER_H
