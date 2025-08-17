#include "data_adapter.h"
#include "../core/utils/logger.h"
#include <thread>
#include <chrono>
#include <random>

// MockDataAdapter implementation
MockDataAdapter::MockDataAdapter() : running_(false), sequence_(0) {}

MockDataAdapter::~MockDataAdapter() {
    stop();
}

void MockDataAdapter::subscribe(const std::string& symbol) {
    current_symbol_ = symbol;
    Logger::get_instance().info("MockDataAdapter subscribed to: " + symbol);
}

void MockDataAdapter::unsubscribe(const std::string& symbol) {
    if (current_symbol_ == symbol) {
        current_symbol_.clear();
        Logger::get_instance().info("MockDataAdapter unsubscribed from: " + symbol);
    }
}

void MockDataAdapter::set_callback(DataCallback callback) {
    callback_ = callback;
}

void MockDataAdapter::start() {
    if (!running_) {
        running_ = true;
        worker_thread_ = std::thread(&MockDataAdapter::generate_test_data, this);
        Logger::get_instance().info("MockDataAdapter started");
    }
}

void MockDataAdapter::stop() {
    if (running_) {
        running_ = false;
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }
        Logger::get_instance().info("MockDataAdapter stopped");
    }
}

void MockDataAdapter::generate_test_data() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> price_dist(100.0, 200.0);
    std::uniform_int_distribution<> volume_dist(100000, 1000000);
    
    double base_price = 150.0;
    
    while (running_ && !current_symbol_.empty()) {
        // Generate realistic price movement
        std::uniform_real_distribution<> change_dist(-2.0, 2.0);
        double change = change_dist(gen);
        base_price += change;
        
        // Ensure price stays positive
        base_price = std::max(base_price, 10.0);
        
        double high = base_price + std::abs(change) * 0.5;
        double low = base_price - std::abs(change) * 0.5;
        
        MarketData data(
            current_symbol_,
            "2024-01-01 09:30:00", // Simplified timestamp for mock
            base_price - change * 0.5,
            high,
            low,
            base_price,
            volume_dist(gen)
        );
        
        if (callback_) {
            callback_(data);
        }
        
        // Generate data every 500ms
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}