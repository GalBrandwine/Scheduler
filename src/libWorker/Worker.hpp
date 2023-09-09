#pragma once
#include <memory>
#include "IBroker.hpp"
#include "spdlog/spdlog.h"
#include <thread>
#include <mutex>
#include <condition_variable>
class Worker
{
    static int workers;

private:
    int m_WorkerNum{0};
    int m_WorkAccomplished{0};
    std::shared_ptr<IBroker> m_MessageBroker;
    std::thread m_WorkingThread;
    std::mutex m_WorkMutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_keepWorking{true};
    bool m_HaveWork{false};
    void doWork();

public:
    int GetWorksAccomplished() const { return m_WorkAccomplished; };
    Worker(std::shared_ptr<IBroker> messageBroker);
    ~Worker();
    Worker(const Worker &other) // II. copy constructor
    {
        spdlog::warn("Worker copy constructor invoked");
        m_MessageBroker = other.m_MessageBroker;
        m_WorkAccomplished = other.m_WorkAccomplished;
    }

    Worker &operator=(const Worker &other) // III. copy assignment
    {
        if (this == &other)
            return *this;

        spdlog::warn("Worker copy assignment invoked");

        return *this;
    }
};
