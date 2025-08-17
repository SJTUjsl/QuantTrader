#include "data/market_data_feed.h"
#include "core/utils/logger.h"
#include "core/message_bus.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    Logger::get_instance().init("logs/market_data_feed.log");
    
    Logger::get_instance().info("Starting market data feed test...");
    
    // 创建行情数据订阅者
    MessageBus::get_instance().subscribe("market_data", [](const std::string& topic, const std::string& data) {
        Logger::get_instance().info("Subscriber received: " + data);
    });
    
    // 创建行情数据流
    MarketDataFeed feed;
    
    // 订阅并启动
    feed.subscribe("AAPL");
    feed.start();
    
    Logger::get_instance().info("Subscribed to AAPL, waiting for 10 data points...");
    
    // 等待接收10条数据（约5秒）
    std::this_thread::sleep_for(std::chrono::seconds(6));
    
    feed.stop();
    feed.unsubscribe("AAPL");
    
    Logger::get_instance().info("Market data feed test completed");
    
    return 0;
}