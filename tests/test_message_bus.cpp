#include "../backend/core/message_bus.h"
#include "../backend/core/utils/logger.h"
#include <iostream>
#include <thread>
#include <chrono>

class TestModuleA {
public:
    TestModuleA() {
        MessageBus::get_instance().subscribe("market_data", [this](const std::string& topic, const std::string& data) {
            on_market_data(data);
        });
    }
    
    void on_market_data(const std::string& data) {
        Logger::get_instance().info("Module A received market data: " + data);
        // Publish processed signal
        MessageBus::get_instance().publish("processed_signal", "A_PROCESSED_" + data);
    }
};

class TestModuleB {
public:
    TestModuleB() {
        MessageBus::get_instance().subscribe("processed_signal", [this](const std::string& topic, const std::string& data) {
            on_processed_signal(data);
        });
    }
    
    void on_processed_signal(const std::string& data) {
        Logger::get_instance().info("Module B received processed signal: " + data);
    }
};

int main() {
    Logger::get_instance().init("logs/message_bus.log");
    
    TestModuleA module_a;
    TestModuleB module_b;
    
    Logger::get_instance().info("Starting message bus test...");
    
    // Simulate publishing some data
    MessageBus::get_instance().publish("market_data", "AAPL_150.25");
    MessageBus::get_instance().publish("market_data", "GOOGL_2750.10");
    
    // Wait for processing to complete
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    Logger::get_instance().info("Message bus test completed");
    
    return 0;
}