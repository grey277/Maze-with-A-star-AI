//
// Created by grey on 27.05.16.
//

#ifndef QUAKEWITHSOCKETS_CLIENT_H
#define QUAKEWITHSOCKETS_CLIENT_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

#include "../include/message.h"
#include "../game_include/Map.h"
#include "../game_include/Renderer.h"

using boost::asio::ip::tcp;

typedef std::deque<message> message_queue;

class client {
public:
    client(boost::asio::io_service& io_service,
    tcp::resolver::iterator endpoint_iterator, Map *map, Renderer *renderer) : io_service_(io_service), socket_(io_service), _map(map), _renderer(renderer) {
        connect(endpoint_iterator);
    }

    void write(const message& msg)
    {
        io_service_.post(
                [this, msg]()
                {
                    bool write_in_progress = !write_msgs_.empty();
                    write_msgs_.push_back(msg);
                    if (!write_in_progress)
                    {
                        do_write();
                    }
                });
    }

    void close()
    {
        io_service_.post([this]() { socket_.close(); });
    }

private:

    void connect(tcp::resolver::iterator endpoint_iterator)
    {
        boost::asio::async_connect(socket_, endpoint_iterator,
                                   [this](boost::system::error_code ec, tcp::resolver::iterator)
                                   {
                                       if (!ec)
                                       {
                                           do_read_header();
                                       }
                                   });
    }

    void do_read_header()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), message::header_length),
                                [this](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        do_read_body();
                                    }
                                    else
                                    {
                                        socket_.close();
                                    }
                                });
    }

    void do_read_body()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                [this](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec)
                                    {
                                        if(read_msg_.messageType() == message::type::mapm) {
                                            _map->changeMap(read_msg_.body(), read_msg_.body_length());
                                            _renderer->render();
                                            do_read_header();
                                        }
                                    }
                                    else
                                    {
                                        socket_.close();
                                    }
                                });
    }

    void do_write()
    {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().length()),
                                 [this](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec)
                                     {
                                         write_msgs_.pop_front();
                                         if (!write_msgs_.empty())
                                         {
                                             do_write();
                                         }
                                     }
                                     else
                                     {
                                         socket_.close();
                                     }
                                 });
    }




    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    message read_msg_;
    message_queue write_msgs_;
    Map *_map;
    Renderer *_renderer;
};

#endif //QUAKEWITHSOCKETS_CLIENT_H
