#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <memory>

enum class LogLevel {
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static Logger& get_instance();
    
    void init(const std::string& log_file_path);
    void log(LogLevel level, const std::string& message);
    
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);

private:
    Logger() = default;
    ~Logger();
    
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    std::string level_to_string(LogLevel level);
    std::string get_current_timestamp();
    
    std::ofstream log_file_;
    std::mutex mutex_;
    bool initialized_ = false;
};