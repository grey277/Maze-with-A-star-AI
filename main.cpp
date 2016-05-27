#include <iostream>

using namespace std;


#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "server.h"
#include "client.h"

int main() {
    try
    {
        boost::asio::io_service io_service;
        server server(io_service);
        io_service.run();

        boost::asio::io_service io_service2;
        client client(io_service2, "localhost");
        client.reciveData("a");
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}