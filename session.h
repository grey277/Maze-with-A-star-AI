//
// Created by grey on 27.05.16.
//

#ifndef QUAKEWITHSOCKETS_SESSION_H
#define QUAKEWITHSOCKETS_SESSION_H

#include <boost/enable_shared_from_this.hpp>
#include "message.h"
#include "room.h"

class session : public participant, public std::enable_shared_from_this<session>{
public:
    session(tcp::socket socket, room& room) : socket_(socket), room_(room) { }

    void deliver(const message& msg) {
        write_msg_ = msg;
    }

    void start() {
        room_.join(shared_from_this());

    }

private:

    void do_read_header() {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), message::header_length),
        [this, self](boost::system::error_code ec, std::size_t){
           if(!ec && read_msg_.decode_header()) {
               do_read_body();
           }
           else {
               room_.leave(shared_from_this());
           }
        });
    }

    void do_read_body() {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](boost::system::error_code ec, std::size_t){
           if(!ec) {
               room_.deliver(read_msg_);
               do_read_header();
           } else {
               room_.leave(shared_from_this());
           }
        });
    }
    void write() {
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
        boost::asio::buffer(write_msg_.data(), write_msg_.length()),
        [this, self](boost::system::error_code ec, std::size_t) {
           if(!ec) {

           } else {
               room_.leave(shared_from_this());
           }
        });
    }

    tcp::socket socket_;
    room& room_;
    message write_msg_;
    message read_msg_;
};



#endif //QUAKEWITHSOCKETS_SESSION_H
