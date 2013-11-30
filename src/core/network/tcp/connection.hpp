#pragma once

#include <core/network/writeBuffer.hpp>
#include <common/mockable.hpp>
#include <common/asio.hpp>

#include <boost/noncopyable.hpp>
#include <functional>

namespace eMU
{
namespace core
{
namespace network
{
namespace tcp
{

class Connection: boost::noncopyable
{
public:
    typedef std::function<void(Connection&)> EventCallback;
    typedef std::shared_ptr<Connection> Pointer;
    typedef std::shared_ptr<asio::ip::tcp::socket> SocketPointer;

    Connection(asio::io_service &ioService);
    Connection(SocketPointer socket);
    virtual ~Connection();

    Payload& getReadPayload();

    MOCKABLE void setReceiveEventCallback(const EventCallback &callback);
    MOCKABLE void setCloseEventCallback(const EventCallback &callback);

    MOCKABLE void disconnect();
    MOCKABLE void close();
    MOCKABLE void send(const Payload &payload);
    MOCKABLE void queueReceive();
    MOCKABLE bool connect(const boost::asio::ip::tcp::endpoint &endpoint);
    MOCKABLE bool isOpen() const;

    bool operator==(const Connection &connection) const;

private:
    Connection();

    void queueSend();

    void receiveHandler(const boost::system::error_code &errorCode, size_t bytesTransferred);
    void sendHandler(const boost::system::error_code &errorCode, size_t bytesTransferred);
    void errorHandler(const boost::system::error_code &errorCode, const std::string &operationName);

    SocketPointer socket_;

    Payload readPayload_;
    WriteBuffer writeBuffer_;

    asio::io_service::strand strand_;
    bool closeOngoing_;

    EventCallback receiveEventCallback_;
    EventCallback closeEventCallback_;
};

}
}
}
}
