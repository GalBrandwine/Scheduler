#include "Worker.hpp"

int Worker::workers{0};
Worker::Worker(std::shared_ptr<IBroker> messageBroker) : m_MessageBroker(messageBroker)
{
    m_WorkerNum = workers++;
    spdlog::info("Worker  #{} constructed!", m_WorkerNum);
    m_WorkingThread = std::thread(std::bind(&Worker::doWork, this));
    m_MessageBroker->Subscribe(MESSAGE_TYPE::TICK_MESSAGE, [&]()
                               {
        {
            std::lock_guard<std::mutex> lock(m_WorkMutex);
            m_HaveWork = true;
        }
        m_cv.notify_one(); });
}

Worker::~Worker()
{
    m_HaveWork = true;
    m_keepWorking = false;
    m_cv.notify_all();
    if (m_WorkingThread.joinable())
    {
        m_WorkingThread.join();
    }
};
void Worker::doWork()
{
    while (m_keepWorking)
    {
        // Wait until main() sends data
        std::unique_lock lk(m_WorkMutex); // Reminder: std::lock_guard, cant work with conditional variables
        m_cv.wait(lk, [&]
                  { return m_HaveWork; });
        if (!m_keepWorking)
        {
            spdlog::debug("Worker stopped, breaking the loop");
            break;
        }

        spdlog::debug("Worker #{} is working [Works accomplished: {}]", m_WorkerNum, m_WorkAccomplished++);
        m_HaveWork = false;
    }
}