#include "dummy_strategy.h"
#include "../core/utils/logger.h"

DummyStrategy::DummyStrategy() : StrategyBase("DummyStrategy"), counter_(0) {
    last_signal_ = Signal(SignalType::HOLD, "", 0, 0, "");
}

void DummyStrategy::init() {
    Logger::get_instance().info("DummyStrategy initialized");
    counter_ = 0;
    reset();
}

void DummyStrategy::reset() {
    Logger::get_instance().info("DummyStrategy reset");
    last_signal_ = Signal(SignalType::HOLD, "", 0, 0, "");
}

void DummyStrategy::on_bar(const MarketData& data) {
    counter_++;
    Logger::get_instance().info("DummyStrategy processing bar " + std::to_string(counter_) + 
                               " for " + data.symbol + " at " + std::to_string(data.close));
}

Signal DummyStrategy::generate_signal() {
    // 简单的交替信号：BUY, SELL, HOLD 循环
    SignalType type = SignalType::HOLD;
    
    switch (counter_ % 3) {
        case 0:
            type = SignalType::BUY;
            break;
        case 1:
            type = SignalType::SELL;
            break;
        case 2:
            type = SignalType::HOLD;
            break;
    }
    
    Signal signal(type, current_data_.symbol, current_data_.close, 100, current_data_.timestamp);
    
    std::string signal_type = (type == SignalType::BUY) ? "BUY" :
                             (type == SignalType::SELL) ? "SELL" : "HOLD";
    Logger::get_instance().info("DummyStrategy generated signal: " + signal_type +
                               " for " + current_data_.symbol);
    
    return signal;
}