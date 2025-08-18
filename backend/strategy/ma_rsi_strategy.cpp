#include "ma_rsi_strategy.h"
#include "../core/utils/logger.h"
#include <cmath>
#include <algorithm>

MA_RSI_Strategy::MA_RSI_Strategy(int ma_period, double rsi_threshold)
    : StrategyBase("MA_RSI_Strategy"), 
      ma_period_(ma_period), 
      rsi_threshold_(rsi_threshold),
      current_ma_(0.0),
      current_rsi_(50.0) {
    Logger::get_instance().info("MA_RSI_Strategy created with MA period: " + std::to_string(ma_period) + 
                              ", RSI threshold: " + std::to_string(rsi_threshold));
}

void MA_RSI_Strategy::init() {
    Logger::get_instance().info("MA_RSI_Strategy initialized");
    reset();
}

void MA_RSI_Strategy::reset() {
    Logger::get_instance().info("MA_RSI_Strategy reset");
    price_history_.clear();
    current_ma_ = 0.0;
    current_rsi_ = 50.0;
}

void MA_RSI_Strategy::on_bar(const MarketData& data) {
    // Save close price to history
    price_history_.push_back(data.close);
    
    // Limit history size to prevent memory growth
    if (price_history_.size() > 1000) {
        price_history_.erase(price_history_.begin());
    }
    
    // Calculate indicators if sufficient data available
    if (has_sufficient_data()) {
        current_ma_ = calculate_ma(price_history_, ma_period_);
        current_rsi_ = calculate_rsi(price_history_, 14);
        
        Logger::get_instance().info("MA_RSI_Strategy: " + data.symbol + 
                                  " Close: " + std::to_string(data.close) +
                                  " MA: " + std::to_string(current_ma_) +
                                  " RSI: " + std::to_string(current_rsi_));
    }
}

Signal MA_RSI_Strategy::generate_signal() {
    if (!has_sufficient_data()) {
        return Signal(SignalType::HOLD, current_data_.symbol, current_data_.close, 0, current_data_.timestamp);
    }
    
    double price = current_data_.close;
    double ma = current_ma_;
    double rsi = current_rsi_;
    
    // Signal generation logic: combine MA and RSI
    SignalType signal_type = SignalType::HOLD;
    int quantity = 100; // default trade volume
    
    // Buy signal: price above MA and RSI oversold (< threshold)
    if (price > ma * 1.01 && rsi < rsi_threshold_) {
        signal_type = SignalType::BUY;
        Logger::get_instance().info("MA_RSI_Strategy: BUY signal generated");
    }
    // Sell signal: price below MA and RSI overbought (> 100-threshold)
    else if (price < ma * 0.99 && rsi > (100.0 - rsi_threshold_)) {
        signal_type = SignalType::SELL;
        Logger::get_instance().info("MA_RSI_Strategy: SELL signal generated");
    }
    // Take profit and stop loss for existing positions
    else if (has_position()) {
        double returns = calculate_returns();
        if (returns > 0.05) { // 5% take profit
            signal_type = SignalType::SELL;
            Logger::get_instance().info("MA_RSI_Strategy: Take profit signal");
        } else if (returns < -0.03) { // 3% stop loss
            signal_type = SignalType::SELL;
            Logger::get_instance().info("MA_RSI_Strategy: Stop loss signal");
        }
    }
    
    return Signal(signal_type, current_data_.symbol, price, quantity, current_data_.timestamp);
}

void MA_RSI_Strategy::set_ma_period(int period) {
    if (period > 0) {
        ma_period_ = period;
        Logger::get_instance().info("MA period updated to: " + std::to_string(period));
    }
}

void MA_RSI_Strategy::set_rsi_threshold(double threshold) {
    if (threshold > 0.0 && threshold < 100.0) {
        rsi_threshold_ = threshold;
        Logger::get_instance().info("RSI threshold updated to: " + std::to_string(threshold));
    }
}

double MA_RSI_Strategy::calculate_ma(const std::vector<double>& prices, int period) const {
    if (prices.size() < static_cast<size_t>(period)) return 0.0;
    
    double sum = 0.0;
    for (size_t i = prices.size() - period; i < prices.size(); ++i) {
        sum += prices[i];
    }
    return sum / period;
}

double MA_RSI_Strategy::calculate_rsi(const std::vector<double>& prices, int period) const {
    if (prices.size() < static_cast<size_t>(period + 1)) return 50.0;
    
    std::vector<double> gains;
    std::vector<double> losses;
    
    // Calculate price changes
    for (size_t i = 1; i < prices.size(); ++i) {
        double change = prices[i] - prices[i-1];
        gains.push_back(change > 0.0 ? change : 0.0);
        losses.push_back(change < 0.0 ? -change : 0.0);
    }
    
    if (gains.size() < static_cast<size_t>(period)) return 50.0;
    
    // Calculate initial average gains and losses
    double avg_gain = 0.0;
    double avg_loss = 0.0;
    for (int i = 0; i < period; ++i) {
        avg_gain += gains[gains.size() - period + i];
        avg_loss += losses[losses.size() - period + i];
    }
    avg_gain /= period;
    avg_loss /= period;
    
    if (avg_loss == 0.0) return 100.0;
    
    double rs = avg_gain / avg_loss;
    return 100.0 - (100.0 / (1.0 + rs));
}