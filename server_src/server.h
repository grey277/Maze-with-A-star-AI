#ifndef QUAKEWITHSOCKETS_SERVER_H
#define QUAKEWITHSOCKETS_SERVER_H

#include "message.h"

class participant {
public:
    virtual ~participant() { }

    virtual void deliver(const message &msg) = 0;
};

typedef std::shared_ptr<participant> participant_ptr;

#include "session.h"
#include "Map.h"

using boost::asio::ip::tcp;

class server {
public:
    server(boost::asio::io_service &io_service, const tcp::endpoint &endpoint, boost::shared_ptr<Map> map)
            : acceptor_(io_service, endpoint),
              socket_(io_service), _map(map), _room(_map) {
        acc();
    }

    room *getRoom() {
        return &_room;
    }

private:
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    boost::shared_ptr<Map> _map;
    room _room;

    void acc() {
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code ec) {
                                   if (!ec) {
                                       std::make_shared<session>(std::move(socket_), _room)->start();
                                   }

                                   acc();
                               });
    }
};

#endif //QUAKEWITHSOCKETS_SERVER_H
