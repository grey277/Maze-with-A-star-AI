//
// Created by grey on 27.05.16.
//

#ifndef QUAKEWITHSOCKETS_CLIENT_H
#define QUAKEWITHSOCKETS_CLIENT_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

using boost::asio::ip::tcp;

class client {
public:
    client(boost::shared_ptr<boost::asio::io_service> io_service, std::string host) : socket(*io_service), resolver(*io_service),
        query(tcp::v4(), "localhost", "4009"), endpoint_iterator(resolver.resolve(query)) { }

    void reciveData(std::string data) {
        boost::asio::connect(socket, endpoint_iterator);
        while(true) {
            boost::array<char, 128> buf;
            boost::system::error_code error_code;
            size_t len = socket.read_some(boost::asio::buffer(buf), error_code);

            if(error_code == boost::asio::error::eof)
                break;
            else if(error_code)
                throw boost::system::system_error(error_code);

            std::cout.write(buf.data(), len);
        }
    }


private:
    tcp::resolver resolver;
    tcp::resolver::query query;
    tcp::resolver::iterator endpoint_iterator;
    tcp::socket socket;
};

#endif //QUAKEWITHSOCKETS_CLIENT_H
