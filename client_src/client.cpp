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
    Map m(60, 30);
    Renderer r(&m);
    client c(*io_service_client, endpoint_iterator, &m, &r);
    std::thread t([&io_service_client](){ io_service_client->run(); });
    Game game(&c, &m);
    game.addPlayer();

    //while(true) { }

    c.close();
    return 0;
}