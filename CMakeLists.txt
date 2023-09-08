cmake_minimum_required(VERSION 3.12.0)
project(ScheduledSystem VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# ----------------------------------------------------------------
# Broker library
# ----------------------------------------------------------------
add_library(Broker SHARED
  src/libBroker/Scheduler.cpp
  src/libBroker/Broker.cpp)

target_include_directories(Broker PUBLIC
  ${PROJECT_SOURCE_DIR}/src/libBroker
  ${spdlog_SOURCE_DIR}/include
)

target_link_libraries(Broker PUBLIC
  ${spdlog_BINARY_DIR}
)

# Adding target_compile_definition for Broker
target_compile_definitions(Broker PRIVATE GALS_PRIVATE_DEFINITION)

# ---------------------------------Adding unittests to Broker------------------------------------------------------
message("Building unittests for Broker::Scheduler")
add_executable(scheduler_test
  tests/broker_tests/scheduler_test.cpp
)

target_link_libraries(scheduler_test PRIVATE
  Broker
  GTest::gtest_main
)
add_test(scheduler_test scheduler_test)

# ---------------------------------END: Adding unittests to Broker------------------------------------------------------

# ----------------------------------------------------------------
# END: Broker library
# ----------------------------------------------------------------

# ---------------------------------OLD WAY FOR ADDING SPDLOG: Adding spdlog ad precompiled target to Broker------------------------------------------------------
# set(spdlog_DIR lib/spdlog/build)

# if(NOT TARGET spdlog)
# # Stand-alone build
# message("looking for spdlog")
# find_package(spdlog REQUIRED)
# endif()

# # Making spdlog wotk as a shared library - https://github.com/gabime/spdlog/issues/1405
# # Remember to compile with: cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON .. && make -j
# target_link_libraries(Broker PUBLIC spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
# ---------------------------------END: Adding spdlog ad precompiled target------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG 7e635fca68d014934b4af8a1cf874f63989352b7
)
set(SPDLOG_BUILD_SHARED ON CACHE BOOL "" FORCE)

# Very cool way for Fetching gtest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git

  # GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
  GIT_TAG origin/main
)
FetchContent_MakeAvailable(googletest spdlog)

include(GoogleTest)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
