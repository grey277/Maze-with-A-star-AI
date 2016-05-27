#include <iostream>

using namespace std;


#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <thread>

#include "server.h"


void startServer(boost::shared_ptr<boost::asio::io_service> io_service) {
    server server(*io_service, tcp::endpoint(tcp::v4(), 4009));
    io_service->run();
}

int main() {
    try
    {
        boost::shared_ptr<boost::asio::io_service> io_service(new boost::asio::io_service);
        boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work( *io_service ));
        boost::thread_group threads;
        threads.create_thread(boost::bind(&startServer, io_service));

        threads.join_all();

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}