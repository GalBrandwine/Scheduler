#pragma once
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "spdlog/spdlog.h"
#include "IBroker.hpp"
class Scheduler
{
private:
    std::mutex m_mut;
    std::condition_variable m_cv;
    bool m_Ready{false};
    bool m_Processed{false};
    std::atomic<bool> m_keepTicking{true};
    std::thread m_ticksProducerThread, m_ticksConsumerThread;
    int m_uptime{0};
    std::shared_ptr<IBroker> m_MessageBroker;
    /**
     * @brief Ticker is a thread that tick every 1 sec
     *
     */
    void ticksProducer();
    void tickConsumer();

public:
    Scheduler(std::shared_ptr<IBroker> messageBroker);
    Scheduler(/* args */);
    // Following rule of 3:
    // - Since I have implemented my own destructor, I should implement also:
    // - Copyconsturctor
    // - Copy assignment
    ~Scheduler();
    Scheduler(const Scheduler &other) // II. copy constructor
    {
        spdlog::warn("Scheduler copy constructor invoked");
    }

    Scheduler &operator=(const Scheduler &other) // III. copy assignment
    {
        if (this == &other)
            return *this;

        spdlog::warn("Scheduler copy assignment invoked");

        return *this;
    }

    int GetUptime() const { return m_uptime; }
};
