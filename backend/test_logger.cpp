#include "core/utils/logger.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    Logger::get_instance().init("logs/system.log");
    
    Logger::get_instance().info("Logger initialized successfully");
    Logger::get_instance().warn("This is a warning message");
    Logger::get_instance().error("This is an error message");
    
    std::cout << "Logger test completed. Check logs/system.log file for output." << std::endl;
    return 0;
}