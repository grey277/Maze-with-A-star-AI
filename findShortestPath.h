//
// Created by Adrian on 2016-05-27.
//

#ifndef QUAKEWITHSOCKETS_FINDSHORTESTPATH_H
#define QUAKEWITHSOCKETS_FINDSHORTESTPATH_H


#include <iostream>
#include <queue>
#include <cmath>

#include "Game.h"

using namespace std;

class Point {
    int xPos;
    int yPos;

public:
    Point() {
    }

    Point(int xPos, int yPos) {
        this->xPos = xPos;
        this->yPos = yPos;
    }

    Point(const Point &p) {
        xPos = p.xPos;
        yPos = p.yPos;
    }

    void operator=(Point p) {
        xPos = p.xPos;
        yPos = p.yPos;
    }

    bool operator==(Point p) {
        return xPos == p.xPos && yPos == p.yPos;
    }

    int getXPos() const {
        return xPos;
    }

    int getYPos() const {
        return yPos;
    }

    friend ostream &operator<<(ostream &cout, const Point &p) {
        return cout << p.getXPos() << "," << p.getYPos();
    }
};

class Node {
    Point pos;

    int traveled;// distance to current Node
    int priority;

public:
    Node(int x, int y, int t, int p) {
        pos = Point(x, y);
        traveled = t;
        priority = p;
    }

    Node(Point point, int t, int p) {
        pos = point;
        traveled = t;
        priority = p;
    }

    Node(const Node &node) {
        pos = node.pos;
        traveled = node.traveled;
        priority = node.priority;
    }

    void operator=(Node node) {
        pos = node.pos;
        traveled = node.traveled;
        priority = node.priority;
    }

    bool operator<(const Node& other) const{
        return this->priority > other.priority;
    }

    Point getPos() const { return pos; }

    int getTraveled() const { return traveled; }

    int getPriority() const { return priority; }

    void updatePriority(const Point destination) {
        priority = traveled + estimate(destination) * 10;
    }

    // going strait - 10; going diagonally - 15 which gives 5 points better than strait
    void nextTraveled(const int &direction) {
        traveled += direction  % 2 == 0 ? 10 : 15;
    }

    // Estimation distance from point to final point
    const int estimate(const Point destination) const {
        return (int) (sqrt(destination.getXPos() - pos.getXPos() * destination.getXPos() - pos.getXPos() + destination.getYPos() - pos.getYPos() * destination.getYPos() - pos.getYPos()));
    }
};

class FindShortestPath : Object{
    const int horizontalSize;
    const int verticalSize;
    type** map;
    int** closedNodesMap; // map of visited Nodes
    int** openNodesMap; // map of not visited Nodes
    int** directionMap; // map of directions
    const static int dir = 8; // number of directions


    const Point directions[dir];
public:
    FindShortestPath(int horizontalSize, int verticalSize, type** map) : horizontalSize(horizontalSize), verticalSize(verticalSize), map(map),
                                                                         directions{Point(1, 0), Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 0), Point(-1, -1), Point(0, -1), Point(1, -1)}    {



        closedNodesMap = new int*[horizontalSize];
        openNodesMap = new int*[horizontalSize];
        directionMap = new int*[horizontalSize];

        for(int i = 0; i < horizontalSize; i++){
            closedNodesMap[i] = new int[verticalSize];
            openNodesMap[i] = new int[verticalSize];
            directionMap[i] = new int[verticalSize];
        }

        for (int y = 0; y < verticalSize; y++) {
            for (int x = 0; x < horizontalSize; x++) {
                closedNodesMap[x][y] = 0;
                openNodesMap[x][y] = 0;
            }
        }
    }

    string pathFind(const Point start, const Point finish) {
        static priority_queue<Node> queue[2]; // list of not visited Nodes
        static int queueIndex = 0;
        static int x, y;
        static char c;

        for (y = 0; y < verticalSize; y++) {
            for (x = 0; x < horizontalSize; x++) {
                closedNodesMap[x][y] = 0;
                openNodesMap[x][y] = 0;
            }
        }

        Node n0(start, 0, 0);
        n0.updatePriority(finish);
        queue[queueIndex].push(n0);

        while (!queue[queueIndex].empty()) {

            n0 = Node(queue[queueIndex].top());
            queue[queueIndex].pop();

            x = n0.getPos().getXPos();
            y = n0.getPos().getYPos();

            openNodesMap[x][y] = 0;
            closedNodesMap[x][y] = 1;

            if (x == finish.getXPos() && y == finish.getYPos()) {
                string path = "";
                int currentDirection;
                while (!(x == start.getXPos() && y == start.getYPos())) {
                    currentDirection = directionMap[x][y];
                    c = '0' + (char) ((currentDirection + dir / 2) % dir);
                    path = c + path;
                    x += directions[currentDirection].getXPos();
                    y += directions[currentDirection].getYPos();
                }
                return path;
            }

            // make moves in all directions
            for (int currentDirection = 0; currentDirection < dir; currentDirection++) {
                Point nextDirection(x + directions[currentDirection].getXPos(),
                                    y + directions[currentDirection].getYPos());

                if (nextDirection.getXPos() >= 0 && nextDirection.getXPos() <= horizontalSize - 1
                    && nextDirection.getYPos() >= 0 && nextDirection.getYPos() <= verticalSize - 1
                    && map[nextDirection.getXPos()][nextDirection.getYPos()] != WALL
                    && closedNodesMap[nextDirection.getXPos()][nextDirection.getYPos()] != 1) {

                    Node m0(nextDirection, n0.getTraveled(), n0.getPriority());
                    m0.nextTraveled(currentDirection);
                    m0.updatePriority(finish);

                    if (openNodesMap[nextDirection.getXPos()][nextDirection.getYPos()] == 0) {
                        openNodesMap[nextDirection.getXPos()][nextDirection.getYPos()] = m0.getPriority();
                        queue[queueIndex].push(m0);

                        directionMap[nextDirection.getXPos()][nextDirection.getYPos()] =
                                (currentDirection + dir / 2) % dir; //update direction map
                    }
                    else if (openNodesMap[nextDirection.getXPos()][nextDirection.getYPos()] > m0.getPriority()) {
                        // update the priority info in openNodesMap
                        openNodesMap[nextDirection.getXPos()][nextDirection.getYPos()] = m0.getPriority();
                        // update the parent direction info in directionMap
                        directionMap[nextDirection.getXPos()][nextDirection.getYPos()] =
                                (currentDirection + dir / 2) % dir; //update direction map

                        while (!(queue[queueIndex].top().getPos().getXPos() == nextDirection.getXPos() &&
                                 queue[queueIndex].top().getPos().getYPos() == nextDirection.getYPos())) {
                            queue[1 - queueIndex].push(queue[queueIndex].top());
                            queue[queueIndex].pop();
                        }
                        queue[queueIndex].pop();


                        if (queue[queueIndex].size() > queue[1 - queueIndex].size()) queueIndex = 1 - queueIndex;
                        while (!queue[queueIndex].empty()) {
                            queue[1 - queueIndex].push(queue[queueIndex].top());
                            queue[queueIndex].pop();
                        }
                        queueIndex = 1 - queueIndex;
                        queue[queueIndex].push(m0); // add the better Node instead
                    }
                }
            }
        }
        return ""; // no route found
    }

    void printTable() {
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
                    case ENEMY:
                        __throw_domain_error("cannot resolve symbol"); // todo
                        break;
                    case BOT:
                        cout << "S"; //start
                        break;
                    case NOTHING:
                        cout << ".";
                        break;
                    case ITEM:
                        cout << "I";
                        break;
                    case ROUTE:
                        cout << "R";
                        break;
                    default:
                        __throw_domain_error("cannot resolve symbol");
                }
            }
            cout << endl;
        }
    }

    void makeTable(string route, Point start) {
        char c;
        int x = start.getXPos();
        int y = start.getYPos();
        map[x][y] = BOT;
        for (unsigned int i = 0; i < route.length(); i++) {
            c = route.at(i);
            x = x + directions[atoi(&c)].getXPos();
            y = y + directions[atoi(&c)].getYPos();
            map[x][y] = ROUTE;
        }
        map[x][y] = PLAYER;
    }

    void printMapInfo(Point start, Point finish) {
        cout << "Map Size (x,y): " << horizontalSize << "," << verticalSize << endl;
        cout << "Start: " << start << endl;
        cout << "Finish: " << finish << endl;
    }



};



/***********************************************************************************************************************
 *
 *
 * przyklad uzycia
 *
 *
 ***********************************************************************************************************************

    Point start(1, 1), finish(6, 8);
    int horizontalSize = 15, verticalSize = 10;

    Map map(horizontalSize, verticalSize, 1, 1, 6, 8);
    map.makeMap();

    FindShortestPath find (horizontalSize, verticalSize, map.getMap());

    find.printMapInfo(start, finish);

    string route = find.pathFind(start, finish);
    if (route == "")
        cout << "cannot fing route" << endl;
    else
        cout << "Route: " << route << endl;
    cout << "Route length: " << route.length() << endl << endl;

    if (route.length() > 0) {
        find.makeTable(route, start);
        find.printTable();
    }

 */
#endif //QUAKEWITHSOCKETS_FINDSHORTESTPATH_H
