#include "core/message_bus.h"
#include "core/utils/logger.h"
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
        // 发布处理后的信号
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
    
    // 模拟发布一些数据
    MessageBus::get_instance().publish("market_data", "AAPL_150.25");
    MessageBus::get_instance().publish("market_data", "GOOGL_2750.10");
    
    // 等待处理完成
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    Logger::get_instance().info("Message bus test completed");
    
    return 0;
}