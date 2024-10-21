#include "Client.h"

#include <iostream>

namespace
{
    using namespace boost::asio::ip;
}

Client::Client(boost::asio::io_context& io_context, const std::string& ip, const std::string& port)
    : m_socket(io_context)
    , m_resolver(io_context)
    , m_acceptor(io_context, tcp::endpoint(tcp::v4(), std::stoi(port)))
{
    startAccepting();
    connectToClient(ip, port);
}

void Client::onWrite(const boost::system::error_code& ec, std::size_t, const std::string& message)
{
    if (!ec)
    {
        //std::cout << m_clientName << " : " << message << '\n';
    }
    else
    {
        std::cerr << "Error sending message: " << ec.message() << '\n';
    }
}

void Client::sendMessage(const std::string& message)
{
    std::string finalMessage = m_clientName + " : " + message;
    if (finalMessage[finalMessage.size() - 1] != '\n')
    {
        finalMessage += '\n';
    }
    boost::asio::async_write(
        m_socket,
        boost::asio::buffer(finalMessage),
        [this, message](const boost::system::error_code& ec, std::size_t) {
            onWrite(ec, 0, message);
        }
    );
}

void Client::startReceiving()
{
    readMessage();
}

void Client::startAccepting()
{
    m_acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::cout << "Client connected" << '\n';
                m_socket = std::move(socket);
                startReceiving();
            }
            startAccepting();
        }
    );
}

void Client::connectToClient(const std::string& ip, const std::string& port)
{
    auto endpoints = m_resolver.resolve(ip, port);
    boost::asio::async_connect(
        m_socket,
        endpoints,
        [this](const boost::system::error_code& ec, const tcp::endpoint&)
        {
            if (!ec)
            {
                std::cout << "Connected to client" << '\n';
                startReceiving();
            }
        }
    );
}

void Client::onRead(const boost::system::error_code& ec, std::size_t length)
{
    if (!ec)
    {
        std::cout << m_incomingData.substr(0, length);
        m_incomingData.erase(0, length);
        readMessage();
    }
    else
    {
        std::cerr << "Error reading message: " << ec.message() << std::endl;
    }
}

void Client::readMessage()
{
    boost::asio::async_read_until(
        m_socket,
        boost::asio::dynamic_buffer(m_incomingData),
        '\n',
        [this](const boost::system::error_code& ec, std::size_t length)
        {
            onRead(ec, length);
        }
    );
}
