
#include "Scheduler.hpp"

Scheduler::Scheduler(std::shared_ptr<IBroker> messageBroker) : m_MessageBroker(messageBroker)
{
    spdlog::info("Scheduler constructed!");
    m_ticksProducerThread = std::thread(std::bind(&Scheduler::ticksProducer, this));
    m_ticksConsumerThread = std::thread(std::bind(&Scheduler::tickConsumer, this));
}
Scheduler::Scheduler(/* args */)
{
    spdlog::info("Scheduler constructed!");
    m_ticksProducerThread = std::thread(std::bind(&Scheduler::ticksProducer, this));
    m_ticksConsumerThread = std::thread(std::bind(&Scheduler::tickConsumer, this));
}

Scheduler::~Scheduler()
{
    m_Ready = true;
    m_keepTicking = false;
    m_cv.notify_all();
    if (m_ticksProducerThread.joinable())
    {
        m_ticksProducerThread.join();
    }
    if (m_ticksConsumerThread.joinable())
    {
        m_ticksConsumerThread.join();
    }
    spdlog::info("Scheduler deconstructed!");
}

void Scheduler::ticksProducer()
{
    using namespace std::literals::chrono_literals;
    while (m_keepTicking)
    {
        // send data to the worker thread
        {
            std::lock_guard lk(m_mut);
            m_Ready = true;
            spdlog::debug("signals data ready for processing");
        }
        m_cv.notify_one();

        spdlog::debug("Scheduler ticked!");
        m_uptime++;
        std::this_thread::sleep_for(1s);
    }
}

void Scheduler::tickConsumer()
{
    while (m_keepTicking)
    {
        // Wait until main() sends data
        std::unique_lock lk(m_mut); // Reminder: std::lock_guard, cant work with conditional variables
        m_cv.wait(lk, [&]
                  { return m_Ready; });
        if (!m_keepTicking)
        {
            spdlog::debug("consumer stopped, breaking the loop");
            break;
        }

        spdlog::debug("consumed tick");
        m_Ready = false;
        if (m_MessageBroker)
            spdlog::debug("publishing tick message");
        m_MessageBroker->Publish(MESSAGE_TYPE::TICK_MESSAGE);
    }
}