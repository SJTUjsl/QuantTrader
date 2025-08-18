#include "strategy_base.h"
#include "../core/utils/logger.h"
#include <ctime>

StrategyBase::StrategyBase(const std::string& name) 
    : name_(name), active_(false) {
    Logger::get_instance().info("Strategy " + name + " created");
}

void StrategyBase::update_market_data(const MarketData& data) {
    current_data_ = data;
    Logger::get_instance().info("Strategy " + name_ + " updated market data for " + data.symbol);
}

void StrategyBase::update_position(const Position& position) {
    current_position_ = position;
    Logger::get_instance().info("Strategy " + name_ + " updated position for " + position.symbol);
}

double StrategyBase::calculate_returns() const {
    if (current_position_.quantity <= 0 || current_position_.average_price <= 0) {
        return 0.0;
    }
    return (current_data_.close - current_position_.average_price) / current_position_.average_price;
}