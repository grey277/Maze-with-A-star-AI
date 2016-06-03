//
// Created by grey on 31.05.16.
//

#ifndef QUAKEWITHSOCKETS_GAME_H
#define QUAKEWITHSOCKETS_GAME_H

#include "findShortestPath.h"
#include "Map.h"
#include "Player.h"
#include "../client_src/client.h"
#include "Renderer.h"

using namespace std;

class Game : public Object {
public:
    Game(client* client) : _map(30, 30), _client(client) { }

    void addPlayer() {
        _players.push_back(new Player(0, 0, _client, &_map));
    }

    Map getMap() { return _map; }



private:
    Map _map;
    vector<Player*> _players;
    client* _client;
};
#endif //QUAKEWITHSOCKETS_GAME_H
