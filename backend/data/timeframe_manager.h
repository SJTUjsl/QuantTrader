#pragma once

#include "../common/market_data.h"
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <chrono>

enum class TimeFrame {
    SECOND_1 = 1,
    MINUTE_1 = 60,
    MINUTE_5 = 300,
    MINUTE_15 = 900,
    MINUTE_30 = 1800,
    HOUR_1 = 3600,
    DAY_1 = 86400
};

class TimeFrameManager {
public:
    struct Candle {
        double open;
        double high;
        double low;
        double close;
        double volume;
        std::string timestamp;
        std::string symbol;
        
        Candle(const std::string& sym, const std::string& ts, double o, double h, double l, double c, double v)
            : symbol(sym), timestamp(ts), open(o), high(h), low(l), close(c), volume(v) {}
    };
    
    struct TimeFrameData {
        std::vector<Candle> candles;
        std::chrono::system_clock::time_point last_update;
        bool is_ready;
        
        TimeFrameData() : is_ready(false) {}
    };
    
    TimeFrameManager();
    ~TimeFrameManager() = default;
    
    // Register a timeframe for a symbol
    void register_timeframe(const std::string& symbol, TimeFrame timeframe);
    
    // Process tick data and update all registered timeframes
    void process_tick(const MarketData& tick);
    
    // Get completed candles for a specific timeframe and symbol
    std::vector<Candle> get_candles(const std::string& symbol, TimeFrame timeframe, int count = 100) const;
    
    // Check if new candle is ready for a timeframe
    bool has_new_candle(const std::string& symbol, TimeFrame timeframe) const;
    
    // Get the latest candle for a timeframe
    Candle get_latest_candle(const std::string& symbol, TimeFrame timeframe) const;
    
    // Get all registered timeframes for a symbol
    std::vector<TimeFrame> get_registered_timeframes(const std::string& symbol) const;
    
private:
    std::map<std::string, std::map<TimeFrame, TimeFrameData>> symbol_data_;
    
    // Helper functions
    std::chrono::system_clock::time_point parse_timestamp(const std::string& timestamp) const;
    std::string format_time(std::chrono::system_clock::time_point time) const;
    long long get_timeframe_start(std::chrono::system_clock::time_point time, TimeFrame tf) const;
    
    // Update or create candle for timeframe
    void update_candle(const std::string& symbol, TimeFrame tf, const MarketData& tick);
};