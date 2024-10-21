#pragma once
#include <boost\asio.hpp>

using namespace boost::asio;

class Client : public std::enable_shared_from_this<Client>
{

public:
    Client(io_context& io_context, const std::string& ip, const std::string& port);
    void sendMessage(const std::string& message);
    void startReceiving();
    void setName(const std::string& name) { m_clientName = name; }
private:
    void startAccepting();
    void connectToClient(const std::string& ip, const std::string& port);
    void onWrite(const boost::system::error_code& ec, std::size_t, const std::string& message);
    void onRead(const boost::system::error_code& ec, std::size_t length);
    void readMessage();
    io_context m_ioContext;
    ip::tcp::socket m_socket;
    ip::tcp::resolver m_resolver;
    ip::tcp::acceptor m_acceptor;
    std::string m_incomingData;

    std::string m_clientName;
};