//
// Created by raiden on 03.06.16.
//

#ifndef QUAKE_RENDERER_H
#define QUAKE_RENDERER_H

#include <ncurses.h>
#include <exception>

class Renderer {
private:
    Map* _map;
public:
    Renderer(Map* map) : _map(map) {
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
    void printTable() {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_RED);
        //init_pair(2, COLOR_BLACK, COLOR_GREEN);
        for (int y = 1; y < _map->getVerticalSize(); y++) {
            for (int x = 1; x < _map->getHorizontalSize(); x++) {
                switch((*_map)(x,y)){
                    case WALL:
                        attron(COLOR_PAIR(1));
                        mvaddch(y, x, ACS_CKBOARD);
                        //mvaddch(y,x,'O');
                        break;
                    case PLAYER:
                        mvaddch(y,x,'F');
                        break;
                    case ENEMY:
                        mvaddch(y,x,'S');
                        break;
                    case NOTHING:
                        //mvaddch(y,x,'.');
                        break;
                    case DIAMOND:
                        mvaddch(y,x, ACS_DIAMOND);
                        break;
                    default:
                        throw std::invalid_argument("bad wolf");
                }
            }
        }
    }

    void render(){
        clear();
        printTable();
        refresh();
    }
};


#endif //QUAKE_RENDERER_H
