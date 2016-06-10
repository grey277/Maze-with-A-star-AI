#ifndef QUAKEWITHSOCKETS_GAME_H
#define QUAKEWITHSOCKETS_GAME_H

#include "Player.h"

using namespace std;

class Game {
public:
    Game(boost::shared_ptr<client> client, boost::shared_ptr<Map> map) : _map(map), _client(client) {
    }

    void addPlayer() {
        _players.push_back(new Player(2, 2, _client, _map));
    }

private:
    boost::shared_ptr<Map> _map;
    vector<Player *> _players;
    boost::shared_ptr<client> _client;
};

#endif //QUAKEWITHSOCKETS_GAME_H
