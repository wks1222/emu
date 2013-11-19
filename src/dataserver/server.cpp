#include <dataserver/server.hpp>
#include <dataserver/transactions/checkAccountRequestTransaction.hpp>

//#include <core/protocol/packetsExtractor.hpp>
#include <core/common/serviceThreading.hpp>

#include <protocol/dataserver/messageIds.hpp>
#include <protocol/dataserver/decoders/checkAccountRequest.hpp>

#include <glog/logging.h>
#include <boost/lexical_cast.hpp>

namespace eMU
{
namespace dataserver
{

Server::Server(asio::io_service &ioService, database::SqlInterface &sqlInterface, const Configuration &configuration):
    core::network::Server<User>(ioService, configuration.port_, configuration.maxNumberOfUsers_),
    sqlInterface_(sqlInterface) {}

bool Server::onStartup()
{
    return true;
}

void Server::onCleanup() {}

void Server::onAccept(size_t hash)
{
	LOG(INFO) << "hash: " << hash << ", user registered.";
}

void Server::onClose(size_t hash)
{
    LOG(INFO) << "hash: " << hash << ", user closed.";
    usersFactory_.destroy(hash);
}

void Server::handlePacket(size_t hash, core::network::Payload &packet)
{
//    uint16_t messageId = packet.read<uint16_t>();

//    if(messageId == interface::dataserver::ids::CheckAccountRequest)
//    {
//        transactionsManager_.queue(new transactions::CheckAccountRequestTransaction(hash,
//                                                                                    messageSender_,
//                                                                                    interface::dataserver::decoders::CheckAccountRequest(packet),
//                                                                                    sqlInterface_));
//    }
}

}
}

#ifdef eMU_TARGET
int main(int argsCount, char *args[])
{
    if(argsCount < 4)
    {
        LOG(ERROR) << "Invalid command line to start GameServer instance.";
        return 1;
    }

    google::InitGoogleLogging(args[0]);

    eMU::dataserver::Server::Configuration configuration = {0};
    configuration.maxNumberOfUsers_ = boost::lexical_cast<size_t>(args[1]);
    configuration.port_ = boost::lexical_cast<uint16_t>(args[2]);

    size_t maxNumberOfThreads = boost::lexical_cast<size_t>(args[3]);

    boost::asio::io_service service;
    eMU::dataserver::Server server(service, configuration);
    eMU::core::common::ServiceThreading<eMU::dataserver::User> serviceThreading(maxNumberOfThreads, service, server);
    serviceThreading.start();

    return 0;
}
#endif