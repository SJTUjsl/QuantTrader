#include "../backend/core/event_loop.h"
#include "../backend/core/utils/logger.h"
#include <chrono>
#include <iostream>

int main() {
    Logger::get_instance().init("logs/event_loop.log");
    
    EventLoop loop;
    
    // Register a 1-second timer
    auto timer_id = loop.register_timer(std::chrono::seconds(1), []() {
        static int count = 0;
        count++;
        Logger::get_instance().info("Timer fired! Count: " + std::to_string(count));
        
        if (count >= 5) {
            Logger::get_instance().info("Reached 5 iterations, stopping...");
            // This is just a test
        }
    });
    
    Logger::get_instance().info("Starting event loop with 1-second timer...");
    
    loop.start();
    
    // Run for 5 seconds then stop
    std::this_thread::sleep_for(std::chrono::seconds(6));
    loop.stop();
    
    Logger::get_instance().info("Event loop test completed");
    
    return 0;
}