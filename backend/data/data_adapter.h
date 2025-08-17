#pragma once

#include <string>
#include <functional>
#include <memory>
#include <thread>

struct MarketData {
    std::string symbol;
    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
    int64_t volume;
    
    MarketData() : open(0), high(0), low(0), close(0), volume(0) {}
    MarketData(const std::string& sym, const std::string& ts, double o, double h, double l, double c, int64_t v)
        : symbol(sym), timestamp(ts), open(o), high(h), low(l), close(c), volume(v) {}
};

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