#pragma once

#include "data_adapter.h"
#include "../core/event_loop.h"
#include "../core/message_bus.h"
#include <memory>
#include <string>

class MarketDataFeed {
public:
    MarketDataFeed();
    ~MarketDataFeed();
    
    void subscribe(const std::string& symbol);
    void unsubscribe(const std::string& symbol);
    void start();
    void stop();
    
private:
    void on_data_received(const MarketData& data);
    void publish_to_bus(const MarketData& data);
    
    std::unique_ptr<DataAdapter> adapter_;
    bool running_;
    std::string current_symbol_;
};