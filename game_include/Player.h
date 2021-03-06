#ifndef PLAYER_H
#define PLAYER_H

#include <thread>

class Player {
private:
    boost::shared_ptr<client> _client;
    boost::shared_ptr<Map> _map;
    int x;
    int y;
public:
    Player(int startX, int startY, boost::shared_ptr<client> client, boost::shared_ptr<Map> map) : _client(client),
                                                                                                   _map(map) {
        x = startX;
        y = startY;
        _map->setPlayerPosition(x, y);
        startThread();
    }

    void startThread() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            char ch;
            ch = getch();
            if (ch > 0) {
                bool send = false;
                message msg;
                std::string s = "";
                switch (ch) {
                    case 'a':
                        if (_map->canMove(x - 1, y)) {
                            send = true;
                            s += std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(x - 1) + "," +
                                 std::to_string(y) + ",";
                            x--;
                            msg.body_length(s.length());
                            std::memcpy(msg.body(), s.c_str(), msg.body_length());
                        }
                        break;
                    case 'd':
                        if (_map->canMove(x + 1, y)) {
                            send = true;
                            s += std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(x + 1) + "," +
                                 std::to_string(y) + ",";
                            x++;
                            msg.body_length(s.length());
                            std::memcpy(msg.body(), s.c_str(), msg.body_length());
                        }
                        break;
                    case 'w':
                        if (_map->canMove(x, y - 1)) {
                            send = true;
                            s += std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(x) + "," +
                                 std::to_string(y - 1) + ",";
                            y--;
                            msg.body_length(s.length());
                            std::memcpy(msg.body(), s.c_str(), msg.body_length());
                        }
                        break;
                    case 's':
                        if (_map->canMove(x, y + 1)) {
                            send = true;
                            s += std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(x) + "," +
                                 std::to_string(y + 1) + ",";
                            y++;
                            msg.body_length(s.length());
                            std::memcpy(msg.body(), s.c_str(), msg.body_length());
                        }
                        break;
                    default:
                        break;
                }
                if (send) {
                    msg.messageType(message::type::playerPosition);
                    msg.encode_header();
                    _client->write(msg);
                }

            }
        }
    }

    int getX() { return x; }

    int getY() { return y; }
};

#endif //PLAYER_H
