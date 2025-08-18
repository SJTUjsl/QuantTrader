#include "../backend/data/market_data_feed.h"
#include "../backend/core/utils/logger.h"
#include "../backend/core/message_bus.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    Logger::get_instance().init("logs/market_data_feed.log");
    
    Logger::get_instance().info("Starting market data feed test...");
    
    // Create market data subscriber
    MessageBus::get_instance().subscribe("market_data", [](const std::string& topic, const std::string& data) {
        Logger::get_instance().info("Subscriber received: " + data);
    });
    
    // Create market data feed
    MarketDataFeed feed;
    
    // Subscribe and start
    feed.subscribe("AAPL");
    feed.start();
    
    Logger::get_instance().info("Subscribed to AAPL, waiting for 10 data points...");
    
    // Wait for 10 data points (about 5 seconds)
    std::this_thread::sleep_for(std::chrono::seconds(6));
    
    feed.stop();
    feed.unsubscribe("AAPL");
    
    Logger::get_instance().info("Market data feed test completed");
    
    return 0;
}