//
// Created by Adrian on 2016-05-27.
//

#ifndef QUAKEWITHSOCKETS_FINDSHORTESTPATH_H
#define QUAKEWITHSOCKETS_FINDSHORTESTPATH_H


#include <iostream>
#include <queue>
#include <cmath>

#include "Map.h"
#include "Point.h"


using namespace std;

class Node {
    Point pos;

    int traveled;
    // distance to current Node
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

    bool operator<(const Node &other) const {
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
        traveled += direction % 2 == 0 ? 10 : 15;
    }

    // Estimation distance from point to final point
    const int estimate(const Point destination) const {
        return (int) (sqrt(destination.x - pos.x * destination.x
                           - pos.x + destination.y - pos.y * destination.y - pos.y));
    }
};

class FindShortestPath {
    Map *_map;
    int **closedNodesMap; // map of visited Nodes
    int **openNodesMap; // map of not visited Nodes
    int **directionMap; // map of directions
    const static int dir = 4; // number of directions
    //if dir = 8
    //Point(1, 0), Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 0), Point(-1, -1), Point(0, -1), Point(1, -1)
    const Point directions[dir];

    int horizontalSize;
    int verticalSize;

    string path = "";

    Point middleOfPath;

public:
    FindShortestPath(Map *map)
            : _map(map), directions{Point(1, 0), Point(0, 1), Point(-1, 0), Point(0, -1)} {

        horizontalSize = _map->getHorizontalSize();
        verticalSize = _map->getVerticalSize();

        closedNodesMap = new int *[horizontalSize];
        openNodesMap = new int *[horizontalSize];
        directionMap = new int *[horizontalSize];

        for (int i = 0; i < horizontalSize; i++) {
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

    void setMap(Map *map) {
        _map = map;
    }

    Point pathFind(const Point start, const Point finish) {
        priority_queue<Node> queue[2]; // list of not visited Nodes
        int queueIndex = 0;
        int x = 0, y = 0;
        char c;

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

            x = n0.getPos().x;
            y = n0.getPos().y;

            openNodesMap[x][y] = 0;
            closedNodesMap[x][y] = 1;

            if (x == finish.x && y == finish.y) {
                path = "";
                int currentDirection;
                while (!(x == start.x && y == start.y)) {
                    currentDirection = directionMap[x][y];
                    c = '0' + (char) ((currentDirection + dir / 2) % dir);
                    path = c + path;
                    x += directions[currentDirection].x;
                    y += directions[currentDirection].y;
                }
                if (!path.empty()) {
                    c = path.at(0);
                    return Point(start.x + directions[atoi(&c)].x,
                                 start.y + directions[atoi(&c)].y);
                }
                return Point(start.x, start.y);
            }

            // make moves in all directions
            for (int currentDirection = 0; currentDirection < dir; currentDirection++) {
                Point nextDirection(x + directions[currentDirection].x,
                                    y + directions[currentDirection].y);

                if (nextDirection.x >= 0 && nextDirection.x <= horizontalSize - 1
                    && nextDirection.y >= 0 && nextDirection.y <= verticalSize - 1
                    &&
                    _map->operator()(nextDirection.x, nextDirection.y) != WALL //TODO, check operator()
                    && closedNodesMap[nextDirection.x][nextDirection.y] != 1) {

                    Node m0(nextDirection, n0.getTraveled(), n0.getPriority());
                    m0.nextTraveled(currentDirection);
                    m0.updatePriority(finish);

                    if (openNodesMap[nextDirection.x][nextDirection.y] == 0) {
                        openNodesMap[nextDirection.x][nextDirection.y] = m0.getPriority();
                        queue[queueIndex].push(m0);

                        directionMap[nextDirection.x][nextDirection.y] =
                                (currentDirection + dir / 2) % dir; //update direction map
                    }
                    else if (openNodesMap[nextDirection.x][nextDirection.y] > m0.getPriority()) {
                        // update the priority info in openNodesMap
                        openNodesMap[nextDirection.x][nextDirection.y] = m0.getPriority();
                        // update the parent direction info in directionMap
                        directionMap[nextDirection.x][nextDirection.y] =
                                (currentDirection + dir / 2) % dir; //update direction map

                        while (!(queue[queueIndex].top().getPos().x == nextDirection.x &&
                                 queue[queueIndex].top().getPos().y == nextDirection.y)) {
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
        return start; // no route found
    }

    std::list<Point *> *makePatch(Point start) {
        std::list<Point *> *l = new std::list<Point *>;
        char c;
        int x = start.x;
        int y = start.y;
        for (unsigned int i = 0; i < path.length(); i++) {
            c = path.at(i);
            int dirX = (int) (c - 48);
            x = x + directions[dirX].x;
            y = y + directions[dirX].y;
            l->push_back(new Point(x, y));
            if(i == path.length() / 2) {
                middleOfPath = Point(x, y);
            }
        }

        return l;
    }

    Point getMiddle() { return middleOfPath; }
};

#endif //QUAKEWITHSOCKETS_FINDSHORTESTPATH_H
