//
// Created by grey on 27.05.16.
//

#ifndef QUAKEWITHSOCKETS_CLIENT_H
#define QUAKEWITHSOCKETS_CLIENT_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>

using boost::asio::ip::tcp;

class client {
public:
    client(boost::asio::io_service io_service, std::string host) : io_service_(io_service), resolver(io_service),
        query(host), endpoint_iterator(resolver.resolve(query)) { }

    void reciveData(std::string data) {
        tcp::socket socket(io_service_);
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
    boost::asio::io_service io_service_;
    tcp::resolver resolver;
    tcp::resolver::query query;
    tcp::resolver::iterator endpoint_iterator;
};

#endif //QUAKEWITHSOCKETS_CLIENT_H
