#include "timeframe_manager.h"
#include "../core/utils/logger.h"
#include <iomanip>
#include <sstream>

TimeFrameManager::TimeFrameManager() {
    Logger::get_instance().info("TimeFrameManager initialized");
}

void TimeFrameManager::register_timeframe(const std::string& symbol, TimeFrame timeframe) {
    symbol_data_[symbol][timeframe] = TimeFrameData();
    Logger::get_instance().info("Registered timeframe " + std::to_string(static_cast<int>(timeframe)) + 
                              "s for symbol " + symbol);
}

void TimeFrameManager::process_tick(const MarketData& tick) {
    std::string symbol = tick.symbol;
    
    // Process this tick for all registered timeframes for this symbol
    if (symbol_data_.find(symbol) != symbol_data_.end()) {
        for (auto& timeframe_pair : symbol_data_[symbol]) {
            update_candle(symbol, timeframe_pair.first, tick);
        }
    }
}

std::vector<TimeFrameManager::Candle> TimeFrameManager::get_candles(
    const std::string& symbol, TimeFrame timeframe, int count) const {
    
    std::vector<Candle> result;
    
    auto symbol_it = symbol_data_.find(symbol);
    if (symbol_it != symbol_data_.end()) {
        auto tf_it = symbol_it->second.find(timeframe);
        if (tf_it != symbol_it->second.end()) {
            const auto& candles = tf_it->second.candles;
            int start_idx = std::max(0, static_cast<int>(candles.size()) - count);
            
            for (int i = start_idx; i < candles.size(); ++i) {
                result.push_back(candles[i]);
            }
        }
    }
    
    return result;
}

bool TimeFrameManager::has_new_candle(const std::string& symbol, TimeFrame timeframe) const {
    auto symbol_it = symbol_data_.find(symbol);
    if (symbol_it != symbol_data_.end()) {
        auto tf_it = symbol_it->second.find(timeframe);
        if (tf_it != symbol_it->second.end()) {
            return tf_it->second.is_ready;
        }
    }
    return false;
}

TimeFrameManager::Candle TimeFrameManager::get_latest_candle(
    const std::string& symbol, TimeFrame timeframe) const {
    
    auto candles = get_candles(symbol, timeframe, 1);
    if (!candles.empty()) {
        return candles.back();
    }
    
    // Return empty candle if no data
    return Candle(symbol, "", 0.0, 0.0, 0.0, 0.0, 0.0);
}

std::vector<TimeFrame> TimeFrameManager::get_registered_timeframes(const std::string& symbol) const {
    std::vector<TimeFrame> timeframes;
    
    auto symbol_it = symbol_data_.find(symbol);
    if (symbol_it != symbol_data_.end()) {
        for (const auto& timeframe_pair : symbol_it->second) {
            timeframes.push_back(timeframe_pair.first);
        }
    }
    
    return timeframes;
}

std::chrono::system_clock::time_point TimeFrameManager::parse_timestamp(const std::string& timestamp) const {
    std::tm tm = {};
    std::istringstream ss(timestamp);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    
    // Convert tm to time_point
    auto time_t = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time_t);
}

std::string TimeFrameManager::format_time(std::chrono::system_clock::time_point time) const {
    auto time_t = std::chrono::system_clock::to_time_t(time);
    std::tm tm;
    localtime_s(&tm, &time_t);
    
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

long long TimeFrameManager::get_timeframe_start(std::chrono::system_clock::time_point time, TimeFrame tf) const {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
        time.time_since_epoch()).count();
    
    int tf_seconds = static_cast<int>(tf);
    return (seconds / tf_seconds) * tf_seconds;
}

void TimeFrameManager::update_candle(const std::string& symbol, TimeFrame tf, const MarketData& tick) {
    auto& data = symbol_data_[symbol][tf];
    
    auto tick_time = parse_timestamp(tick.timestamp);
    long long current_tf_start = get_timeframe_start(tick_time, tf);
    
    // Check if we need to create a new candle
    if (data.candles.empty()) {
        // First candle for this timeframe
        std::string tf_timestamp = format_time(
            std::chrono::system_clock::from_time_t(current_tf_start));
        
        data.candles.emplace_back(symbol, tf_timestamp, 
                                tick.close, tick.high, tick.low, tick.close, tick.volume);
        data.last_update = tick_time;
        data.is_ready = false;
    } else {
        Candle& current_candle = data.candles.back();
        long long last_tf_start = get_timeframe_start(parse_timestamp(current_candle.timestamp), tf);
        
        if (current_tf_start > last_tf_start) {
            // New timeframe started - create new candle
            std::string tf_timestamp = format_time(
                std::chrono::system_clock::from_time_t(current_tf_start));
            
            data.candles.emplace_back(symbol, tf_timestamp, 
                                    tick.close, tick.high, tick.low, tick.close, tick.volume);
            
            // Mark previous candle as ready
            data.is_ready = true;
            
            // Limit history size
            if (data.candles.size() > 1000) {
                data.candles.erase(data.candles.begin());
            }
        } else {
            // Update existing candle
            current_candle.high = std::max(current_candle.high, tick.high);
            current_candle.low = std::min(current_candle.low, tick.low);
            current_candle.close = tick.close;
            current_candle.volume += tick.volume;
            
            data.is_ready = false;
        }
    }
    
    data.last_update = tick_time;
}