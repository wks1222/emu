#include <loginserver/transactions/loginRequestTransaction.hpp>
#include <protocol/dataserver/encoders/checkAccountRequest.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{
namespace transactions
{

LoginRequestTransaction::LoginRequestTransaction(size_t hash,
                                                 core::network::tcp::ConnectionsManager &connectionsManager,
                                                 core::network::tcp::Connection &dataserverConnection,
                                                 const protocol::loginserver::decoders::LoginRequest &request):
    hash_(hash),
    connectionsManager_(connectionsManager),
    dataserverConnection_(dataserverConnection),
    request_(request) {}

bool LoginRequestTransaction::isValid() const
{
    if(!dataserverConnection_.isOpen())
    {
        LOG(ERROR) << "hash: " << hash_ << ", Connection to dataserver not established!";
        return false;
    }

    return true;
}

void LoginRequestTransaction::handleValid()
{
    protocol::dataserver::encoders::CheckAccountRequest checkAccountRequest(hash_, request_.getAccountId(), request_.getPassword());
    dataserverConnection_.send(checkAccountRequest.getWriteStream().getPayload());
}

void LoginRequestTransaction::handleInvalid()
{
    connectionsManager_.disconnect(hash_);
}

}
}
}
