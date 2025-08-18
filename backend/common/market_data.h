#pragma once

#include <string>

struct MarketData {
    std::string symbol;
    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
    int64_t volume;
    
    MarketData() : open(0.0), high(0.0), low(0.0), close(0.0), volume(0) {}
    
    MarketData(const std::string& symbol_, 
               const std::string& timestamp_, 
               double open_, 
               double high_, 
               double low_, 
               double close_, 
               int64_t volume_)
        : symbol(symbol_), timestamp(timestamp_), 
          open(open_), high(high_), low(low_), close(close_), volume(volume_) {}
};