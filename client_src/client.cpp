//
// Created by grey on 27.05.16.
//

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <mutex>

#include "client.h"
#include "../game_include/Game.h"

int main() {
    boost::shared_ptr<boost::asio::io_service> io_service_client(new boost::asio::io_service);
    //boost::shared_ptr<boost::asio::io_service::work> work_client(new boost::asio::io_service::work( *io_service_client ));
    //threads.create_thread(boost::bind(&startClient, io_service_client));
    tcp::resolver resolver(*io_service_client);
    auto endpoint_iterator = resolver.resolve({"localhost", "4009"});
    //std::mutex Map::lock = new std::mutex;
    Map m(60, 30);
    Renderer r;
    client c(*io_service_client, endpoint_iterator, &m, &r);
    std::thread t([&io_service_client](){ io_service_client->run(); });
    Game game(&c, &m);
    game.addPlayer();
    //char line[message::max_body_length + 1];
    //while (std::cin.getline(line, message::max_body_length + 1))
    //{
    //    message msg;
    //    msg.body_length(std::strlen(line));
    //    std::memcpy(msg.body(), line, msg.body_length());
    //    msg.encode_header();
    //    c.write(msg);
    //}

    while(true) { }

    c.close();
    return 0;
}