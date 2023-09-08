# A Scheduler

In this mini project I will implement a Scheduler.
This scheduler will implement a timer, and components could attach themselves to this timer, for timing notifications.

## This project I will

* Revisit multithreading concurrency
* TDD approach
* Other software design fundamentals
* Also I want to improve cmake understanding
  * And guess what? - I've learned about `FetchContent_Declare` and `FetchContent_MakeAvailable` methods!

## Revisiting Design patterns

In this project I will use the **Publisher-Subscriber** design pattern, since I want attached component to implement messaging pattern.

Ill have one component - Ticker, that will send *tick* message every X minutes.
Ill have many components that will subscribe to Tick messages.
