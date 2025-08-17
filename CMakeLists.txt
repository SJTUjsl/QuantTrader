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
)

# 源文件集合
file(GLOB_RECURSE BACKEND_SOURCES
    "backend/*.cpp"
)

# 创建可执行文件
add_executable(quant-trading-system
    ${BACKEND_SOURCES}
)

# 空项目测试
add_executable(empty-test
    backend/main.cpp
)