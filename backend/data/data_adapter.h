#pragma once

#include <string>
#include <functional>
#include <memory>
#include <thread>

#include "../common/market_data.h"

class DataAdapter {
public:
    using DataCallback = std::function<void(const MarketData& data)>;
    
    virtual ~DataAdapter() = default;
    
    virtual void subscribe(const std::string& symbol) = 0;
    virtual void unsubscribe(const std::string& symbol) = 0;
    virtual void set_callback(DataCallback callback) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    
protected:
    DataCallback callback_;
};

class MockDataAdapter : public DataAdapter {
public:
    MockDataAdapter();
    ~MockDataAdapter() override;
    
    void subscribe(const std::string& symbol) override;
    void unsubscribe(const std::string& symbol) override;
    void set_callback(DataCallback callback) override;
    void start() override;
    void stop() override;
    
private:
    void generate_test_data();
    
    bool running_;
    std::thread worker_thread_;
    std::string current_symbol_;
    int64_t sequence_;
};