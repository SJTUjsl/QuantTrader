#include "../backend/data/data_adapter.h"
#include "../backend/core/utils/logger.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

int main() {
    Logger::get_instance().init("logs/data_adapter.log");
    
    Logger::get_instance().info("Starting data adapter test...");
    
    // Create mock data adapter
    std::unique_ptr<DataAdapter> adapter = std::make_unique<MockDataAdapter>();
    
    // Set up callback for received data
    adapter->set_callback([](const MarketData& data) {
        Logger::get_instance().info("Received market data: " + data.symbol + 
                                   " Price: " + std::to_string(data.close) +
                                   " Volume: " + std::to_string(data.volume));
    });
    
    // Subscribe to stock
    adapter->subscribe("AAPL");
    adapter->start();
    
    // Run for 3 seconds to receive data
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    adapter->stop();
    adapter->unsubscribe("AAPL");
    
    Logger::get_instance().info("Data adapter test completed");
    
    return 0;
}