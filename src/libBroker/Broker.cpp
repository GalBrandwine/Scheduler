#include "Broker.hpp"

void Broker::Publish(MESSAGE_TYPE message_type)
{
    std::lock_guard<std::mutex> lock(m_publishLock);
    for (auto &&subscriber : m_SubscribersPerMessageType[message_type])
    {
        subscriber();
    }
}
void Broker::Subscribe(MESSAGE_TYPE message_type, std::function<void()> action)
{
    m_SubscribersPerMessageType[message_type].push_back(action);
}
Broker::Broker(/* args */)
{
}

Broker::~Broker()
{
}