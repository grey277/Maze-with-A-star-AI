//
// Created by grey on 27.05.16.
//

#ifndef QUAKEWITHSOCKETS_SERVER_H
#define QUAKEWITHSOCKETS_SERVER_H


#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <iostream>

#include "tcp_connection.h"

using boost::asio::ip::tcp;


class server {
public:
    server(boost::asio::io_service& ioservice) : acceptor_(ioservice, tcp::endpoint(tcp::v4(), 4009)){
        start();
    }
private:
    tcp::acceptor acceptor_;
    void start() {
        tcp_connection::pointer new_connection =
                tcp_connection::create(acceptor_.get_io_service());

        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&server::handle_accept, this, new_connection,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->start();
        }

        start();
    }
};


#endif //QUAKEWITHSOCKETS_SERVER_H
