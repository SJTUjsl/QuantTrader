#include "data/data_adapter.h"
#include "core/utils/logger.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    Logger::get_instance().init("logs/data_adapter.log");
    
    Logger::get_instance().info("Starting data adapter test...");
    
    // 创建模拟数据适配器
    std::unique_ptr<DataAdapter> adapter = std::make_unique<MockDataAdapter>();
    
    // 设置回调函数处理接收到的数据
    adapter->set_callback([](const MarketData& data) {
        Logger::get_instance().info("Received market data: " + data.symbol + 
                                   " Price: " + std::to_string(data.close) +
                                   " Volume: " + std::to_string(data.volume));
    });
    
    // 订阅股票
    adapter->subscribe("AAPL");
    adapter->start();
    
    // 运行3秒接收数据
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    adapter->stop();
    adapter->unsubscribe("AAPL");
    
    Logger::get_instance().info("Data adapter test completed");
    
    return 0;
}