#include <exception>
#include <iostream>
#include <thread>
#include <boost/asio/io_context.hpp>
#include "Client.h"

int main(int argc, char** argv)
{
    try
    {
        if (argc != 4)
        {
            std::cerr << "Usage: client <name> <host> <port>\n";
            return 1;
        }
        std::string name, ip, port;
        name = argv[1];
        ip = argv[2];
        port = argv[3];

        boost::asio::io_context io_context;
        Client client(io_context, ip, port);
        client.setName(name);

        std::thread thread(
            [&io_context]()
            {
                io_context.run();
            }
        );
        std::string message;
        while (std::getline(std::cin, message)) {
            client.sendMessage(message);
        }

        thread.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
