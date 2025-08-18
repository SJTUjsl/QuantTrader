cmake_minimum_required(VERSION 3.10)
project(quant-trading-system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/backend
    ${CMAKE_SOURCE_DIR}/backend/core
    ${CMAKE_SOURCE_DIR}/backend/core/utils
    ${CMAKE_SOURCE_DIR}/backend/data
    ${CMAKE_SOURCE_DIR}/backend/strategy
    ${CMAKE_SOURCE_DIR}/backend/execution
    ${CMAKE_SOURCE_DIR}/backend/risk
    ${CMAKE_SOURCE_DIR}/backend/portfolio
    ${CMAKE_SOURCE_DIR}/backend/reporting
    ${CMAKE_SOURCE_DIR}/backend/common
)

# 主程序
add_executable(quant-trading-system
    backend/main.cpp
)

# 测试程序
add_executable(test-logger
    tests/test_logger.cpp
    backend/core/utils/logger.cpp
)

add_executable(test-event-loop
    tests/test_event_loop.cpp
    backend/core/event_loop.cpp
    backend/core/utils/logger.cpp
)

add_executable(test-message-bus
    tests/test_message_bus.cpp
    backend/core/message_bus.cpp
    backend/core/utils/logger.cpp
)

add_executable(test-csv-loader
    tests/test_csv_loader.cpp
    backend/data/csv_loader.cpp
    backend/core/utils/logger.cpp
)

add_executable(test-data-adapter
    tests/test_data_adapter.cpp
    backend/data/data_adapter.cpp
    backend/core/utils/logger.cpp
)

add_executable(test-market-data-feed
    tests/test_market_data_feed.cpp
    backend/data/market_data_feed.cpp
    backend/data/data_adapter.cpp
    backend/core/utils/logger.cpp
    backend/core/message_bus.cpp
)

add_executable(test-strategy-base
    tests/test_strategy_base.cpp
    backend/strategy/strategy_base.cpp
    backend/strategy/dummy_strategy.cpp
    backend/core/utils/logger.cpp
)

# 新增MA_RSI策略测试
add_executable(test-ma-rsi-strategy
    tests/test_ma_rsi_strategy.cpp
    backend/strategy/ma_rsi_strategy.cpp
    backend/strategy/strategy_base.cpp
    backend/core/utils/logger.cpp
)

# 链接线程库
find_package(Threads REQUIRED)
target_link_libraries(test-event-loop Threads::Threads)
target_link_libraries(test-data-adapter Threads::Threads)
target_link_libraries(test-market-data-feed Threads::Threads)
target_link_libraries(test-ma-rsi-strategy Threads::Threads)