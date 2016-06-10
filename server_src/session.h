//
// Created by grey on 27.05.16.
//

#ifndef QUAKEWITHSOCKETS_SESSION_H
#define QUAKEWITHSOCKETS_SESSION_H

#include <exception>

#include <boost/enable_shared_from_this.hpp>
#include "../include/message.h"
#include "../game_include/Map.h"

using boost::asio::ip::tcp;

typedef std::deque<message> message_queue;

class room {
public:
    room(boost::shared_ptr<Map> map) : _map(map) { }
    void join(participant_ptr participant)
    {
        participants_.insert(participant);
        for (auto msg : recent_msgs_)
            participant->deliver(msg);

        message msg;
        msg.body_length(std::strlen(_map->toCharStr()));
        std::memcpy(msg.body(), _map->toCharStr(), msg.body_length());
        msg.messageType(message::type::mapm);
        msg.encode_header();
        participant->deliver(msg);
    }

    void leave(participant_ptr participant)
    {
        participants_.erase(participant);
    }

    void deliver(const message& msg)
    {
        if(msg.messageType() == message::type::playerPosition) {
            std::string s(msg.body());
            std::vector<std::string> tokens;
            char *m = const_cast<char*>(s.c_str());
            char * p;
            p = strtok (m,",");
            while (p != NULL) {
                tokens.push_back(std::string(p));
                p = strtok (NULL, ",");
            }
            if(tokens.size() == 4) {
                _map->updatePlayerPosition(std::stoi(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2]), std::stoi(tokens[3]));
                std::string sMsg = "";
                sMsg += tokens[0] + "," + tokens[1] + "," + tokens[2] + "," + tokens[3] + ",";
                message msgToSend;
                msgToSend.body_length(sMsg.length()); // for null char
                memset(msgToSend.body(), 0, msgToSend.body_length());
                std::memcpy(msgToSend.body(), sMsg.c_str(), msgToSend.body_length());
                msgToSend.messageType(message::type::playerPosition);
                msgToSend.encode_header();
                for (auto participant: participants_)
                    participant->deliver(msgToSend);

            }

            int whoWon = _map->didWon();
            if(whoWon != (int) NOTHING ){
                message message;
                message.body_length(std::to_string(whoWon).length() + 1); // for null char
                std::memcpy(message.body(), std::to_string(whoWon).c_str(), message.body_length());
                message.messageType(message::type::endGame);
                message.encode_header();
                for (auto participant: participants_)
                    participant->deliver(message);
            }
        } else {
            recent_msgs_.push_back(msg);
            while (recent_msgs_.size() > max_recent_msgs)
                recent_msgs_.pop_front();

            for (auto participant: participants_)
                participant->deliver(msg);

            int whoWon = _map->didWon();
            if(whoWon != (int) NOTHING ){
                message message;
                message.body_length(std::to_string(whoWon).length() + 1); // for null char
                std::memcpy(message.body(), std::to_string(whoWon).c_str(), message.body_length());
                message.messageType(message::type::endGame);
                message.encode_header();
                for (auto participant: participants_)
                    participant->deliver(message);
            }
        }
    }


private:
    std::set<participant_ptr> participants_;
    enum { max_recent_msgs = 1 };
    message_queue recent_msgs_;
    boost::shared_ptr<Map> _map;
};

class session : public participant, public std::enable_shared_from_this<session>{
public:
    session(tcp::socket socket, room& room) : socket_(std::move(socket)), room_(room) { }

    void start()
    {
        room_.join(shared_from_this());
        do_read_header();
    }

    void deliver(const message& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }

private:
    void do_read_header()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), message::header_length),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        do_read_body();
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
    }

    void do_read_body()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec)
                                    {
                                        room_.deliver(read_msg_);
                                        do_read_header();
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
    }

    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().length()),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/)
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
                                         room_.leave(shared_from_this());
                                     }
                                 });
    }

    tcp::socket socket_;
    room& room_;
    message read_msg_;
    message_queue write_msgs_;
};



#endif //QUAKEWITHSOCKETS_SESSION_H
