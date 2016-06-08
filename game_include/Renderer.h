//
// Created by raiden on 03.06.16.
//

#ifndef QUAKE_RENDERER_H
#define QUAKE_RENDERER_H

#include <ncurses.h>
#include <exception>

class Renderer {
public:
    Renderer() {
        initscr();
        raw();
        cbreak();
        noecho();
        //nodelay(w, TRUE);
        keypad(stdscr, TRUE);
        curs_set(0);
    }

    ~Renderer() {
        endwin();
    }
    void printTable(Map* mapPointer) {
        for (int y = 1; y < mapPointer->getVerticalSize(); y++) {
            for (int x = 1; x < mapPointer->getHorizontalSize(); x++) {
                switch ((*mapPointer)(x,y)) {
                    case 0:
                        break;
                    case 1:
                        mvaddch(y + 5, x + 5, ACS_CKBOARD);
                        break;
                    default:
                        break;

                }
                //switch((*mapPointer)(x,y)){
                //    case WALL:
                //        mvaddch(y,x,'O');
                //        break;
                //    case PLAYER:
                //        mvaddch(y,x,'F');
                //        break;
                //    case ENEMY:
                //        mvaddch(y,x,'S');
                //        break;
                //    case NOTHING:
                //        mvaddch(y,x,'.');
                //        break;
                //    case ITEM:
                //        mvaddch(y,x,'I');
                //        break;
                //    case SHOOT:
                //        mvaddch(y,x,'-');
                //        break;
                //    default:
                //        throw std::invalid_argument("bad wolf");
                //}
            }
        }
    }

    void render(Map* map){
        clear();
        printTable(map);
        refresh();
    }
};


#endif //QUAKE_RENDERER_H
