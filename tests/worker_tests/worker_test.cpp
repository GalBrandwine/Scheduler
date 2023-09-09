#include "gtest/gtest.h"
#include "Worker.hpp"
#include "Broker.hpp"

TEST(Worker, ManyWorkers)
{
    // Setup
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    auto messageBroker = std::make_shared<Broker>();
    std::vector<std::shared_ptr<Worker>> workers{};
    for (size_t i = 0; i < 1000; i++)
    {
        workers.emplace_back(std::make_shared<Worker>(messageBroker));
    }

    // Run
    messageBroker->Publish(MESSAGE_TYPE::TICK_MESSAGE);
    for (size_t i = 0; i < 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Test
    for (const auto &worker : workers)
    {
        ASSERT_EQ(worker->GetWorksAccomplished(), 1);
    }

    // Run 2
    messageBroker->Publish(MESSAGE_TYPE::TICK_MESSAGE);
    for (size_t i = 0; i < 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Test
    for (const auto &worker : workers)
    {
        ASSERT_EQ(worker->GetWorksAccomplished(), 2);
    }
}
TEST(Worker, DoWorkOnce)
{
    // Setup
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    auto messageBroker = std::make_shared<Broker>();
    Worker worker{messageBroker};

    // Run
    messageBroker->Publish(MESSAGE_TYPE::TICK_MESSAGE);
    for (size_t i = 0; i < 5; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Test
    ASSERT_EQ(worker.GetWorksAccomplished(), 1);
}
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}