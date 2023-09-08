#pragma once
#include <functional>
enum MESSAGE_TYPE
{
    TICK_MESSAGE
};
class IBroker
{
private:
    /* data */
public:
    virtual void Publish(MESSAGE_TYPE message_type) = 0;
    virtual void Subscribe(MESSAGE_TYPE message_type, std::function<void()> action) = 0;
    virtual ~IBroker(){};
};