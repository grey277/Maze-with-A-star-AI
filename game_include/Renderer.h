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
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_RED);
        //init_pair(2, COLOR_BLACK, COLOR_GREEN);
        for (int y = 1; y < mapPointer->getVerticalSize(); y++) {
            for (int x = 1; x < mapPointer->getHorizontalSize(); x++) {
                switch((*mapPointer)(x,y)){
                    case 0:
                        attron(COLOR_PAIR(1));
                        mvaddch(y, x, ACS_CKBOARD);
                        //mvaddch(y,x,'O');
                        break;
                    case 1:
                        mvaddch(y,x,'F');
                        break;
                    case 2:
                        mvaddch(y,x,'S');
                        break;
                    case 4:
                        //mvaddch(y,x,'.');
                        break;
                    case 5:
                        mvaddch(y,x, ACS_DIAMOND);
                        break;
                    default:
                        throw std::invalid_argument("bad wolf");
                }
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
