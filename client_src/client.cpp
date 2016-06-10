#include <boost/asio.hpp>

#include "client.h"
#include "Game.h"

int main() {
    boost::shared_ptr<boost::asio::io_service> io_service_client(new boost::asio::io_service);
    tcp::resolver resolver(*io_service_client);
    auto endpoint_iterator = resolver.resolve({"localhost", "4009"});
    boost::shared_ptr<Map> m(new Map(60, 30));
    boost::shared_ptr<Renderer> r(new Renderer(m));
    boost::shared_ptr<client> c(new client(*io_service_client, endpoint_iterator, m, r));
    std::thread t([&io_service_client]() { io_service_client->run(); });

    Game game(c, m);
    game.addPlayer();

    c->close();
    return 0;
}