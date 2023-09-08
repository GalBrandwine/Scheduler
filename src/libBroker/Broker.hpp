#pragma once
#include "IBroker.hpp"
#include <unordered_map>
#include <vector>
#include <mutex>
class Broker : public IBroker
{
private:
    std::mutex m_publishLock;
    std::unordered_map<MESSAGE_TYPE, std::vector<std::function<void()>>> m_SubscribersPerMessageType{};

public:
    void Publish(MESSAGE_TYPE) override;
    void Subscribe(MESSAGE_TYPE, std::function<void()> action) override;
    Broker(/* args */);
    ~Broker();
};
