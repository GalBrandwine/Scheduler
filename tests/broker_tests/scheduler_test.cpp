#include "gtest/gtest.h"
#include "Scheduler.hpp"
#include "Broker.hpp"
TEST(Scheduler, TickWithMessageBroker)
{
    // Setup
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug

    // Initiate shared message broker
    auto messageBroker = std::make_shared<Broker>();
    auto isGotTicked = false;

    // Subscribe fake subscriber
    messageBroker->Subscribe(MESSAGE_TYPE::TICK_MESSAGE, [&isGotTicked]()
                             {spdlog::info("I got ticked");
                                isGotTicked=true; });

    Scheduler scheduler{messageBroker};

    // Run
    while (scheduler.GetUptime() <= 2)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Test
    ASSERT_TRUE(isGotTicked);
}

TEST(Scheduler, TickerEverySecond)
{
    // Setup
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    Scheduler scheduler{};
    auto start = std::chrono::steady_clock::now();
    std::chrono::milliseconds elapsed{0};

    // Run
    while (scheduler.GetUptime() <= 2)
    {
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Test
    ASSERT_LT(elapsed.count(), 2000);
}
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}