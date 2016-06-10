//
// Created by raiden on 03.06.16.
//

#ifndef QUAKE_RENDERER_H
#define QUAKE_RENDERER_H

#include <ncurses.h>
#include <exception>

class Renderer {
private:
    boost::shared_ptr<Map> _map;
//    34/5
    int winText[35][5] = {{1,0,0,0,0},{0,1,0,0,0},{0,0,1,1,1},{0,1,0,0,0},{1,0,0,0,0},
                          {0,0,0,0,0},{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},
                          {0,0,0,0,0},{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{1,1,1,1,1},
                          {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{1,1,1,1,1},{0,0,0,1,0},
                          {0,0,1,0,0},{0,0,0,1,0},{1,1,1,1,1},{0,0,0,0,0},{1,1,1,1,1},
                          {1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},{0,0,0,0,0},{1,1,1,1,1},
                          {0,1,0,0,0},{0,0,1,0,0}, {0,0,0,1,0},{1,1,1,1,1}};

    int looseText[37][5] = {{1,0,0,0,0},{0,1,0,0,0},{0,0,1,1,1},{0,1,0,0,0},{1,0,0,0,0},
                            {0,0,0,0,0},{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},
                            {0,0,0,0,0},{1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{1,1,1,1,1},
                            {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
                            {1,1,1,1,1},{0,0,0,0,1},{0,0,0,0,1},{0,0,0,0,1},
                            {0,0,0,0,0},{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},
                            {1,1,1,1,1},{0,0,0,0,0},{1,1,1,0,1},{1,0,1,0,1},{1,0,1,0,1},
                            {1,0,1,1,1},{0,0,0,0,0},{1,1,1,1,1},{1,0,1,0,1},{1,0,1,0,1},
                            {1,0,1,0,1}};

public:
    Renderer(boost::shared_ptr<Map> map) : _map(map) {
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
        init_pair(2, COLOR_BLACK, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
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
                        attron(COLOR_PAIR(2));
                        mvaddch(y,x,' ');
                        break;
                    case DIAMOND:
                        attron(COLOR_PAIR(3));
                        mvaddch(y,x, ACS_DIAMOND);
                        break;
                    default:
                        throw std::invalid_argument("bad wolf");
                }
            }
        }
    }

    void render(int x, int y) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_RED);
        switch((*_map)(x,y)){
            case WALL:
                attron(COLOR_PAIR(1));
                mvaddch(y, x, ACS_CKBOARD);
                break;
            case PLAYER:
                mvaddch(y,x,'F');
                break;
            case ENEMY:
                mvaddch(y,x,'S');
                break;
            case NOTHING:
                mvaddch(y,x,' ');
                break;
            case DIAMOND:
                mvaddch(y,x, ACS_DIAMOND);
                break;
            default:
                throw std::invalid_argument("bad wolf");
        }
        refresh();
    }

    void render(){
        clear();
        printTable();
        refresh();
    }

    void printWinTable(){
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        for(int x = 0; x < 35; x++){
            for(int y = 0; y < 5; y++){
                switch (winText[x][y]){
                    case 1:
                        attron(COLOR_PAIR(1));
                        mvaddch(y, x, ACS_CKBOARD );
                        break;
                    case 0:
                        //mvaddch(y, x, ' ' );
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void printLooseTable(){
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        for(int x = 0; x < 37; x++){
            for(int y = 0; y < 5; y++){
                switch (looseText[x][y]){
                    case 1:
                        attron(COLOR_PAIR(1));
                        mvaddch(y, x, ACS_CKBOARD );
                        break;
                    case 0:
                        //mvaddch(y, x, ' ' );
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void printWinText(int whoWon){
        clear();
        if(whoWon == PLAYER)
            printWinTable();
        if(whoWon == ENEMY)
            printLooseTable();
        refresh();
    }
};


#endif //QUAKE_RENDERER_H
