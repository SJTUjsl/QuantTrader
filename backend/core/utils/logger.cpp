#include "logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

Logger& Logger::get_instance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void Logger::init(const std::string& log_file_path) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!initialized_) {
        log_file_.open(log_file_path, std::ios::app);
        if (!log_file_.is_open()) {
            std::cerr << "Failed to open log file: " << log_file_path << std::endl;
            return;
        }
        initialized_ = true;
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!initialized_) {
        std::cerr << "Logger not initialized. Call init() first." << std::endl;
        return;
    }
    
    std::string timestamp = get_current_timestamp();
    std::string level_str = level_to_string(level);
    std::string log_entry = "[" + timestamp + "] [" + level_str + "] " + message;
    
    // Output to console
    std::cout << log_entry << std::endl;
    
    // Output to file
    if (log_file_.is_open()) {
        log_file_ << log_entry << std::endl;
        log_file_.flush();
    }
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warn(const std::string& message) {
    log(LogLevel::WARN, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

std::string Logger::level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARN: return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

std::string Logger::get_current_timestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}