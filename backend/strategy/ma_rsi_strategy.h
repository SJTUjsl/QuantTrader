#pragma once

#include "strategy_base.h"
#include <vector>
#include <memory>

class MA_RSI_Strategy : public StrategyBase {
public:
    MA_RSI_Strategy(int ma_period = 20, double rsi_threshold = 30.0);
    ~MA_RSI_Strategy() override = default;
    
    void init() override;
    void reset() override;
    void on_bar(const MarketData& data) override;
    Signal generate_signal() override;
    
    // Parameter settings
    void set_ma_period(int period);
    void set_rsi_threshold(double threshold);
    
    // Get indicator values
    double get_current_ma() const { return current_ma_; }
    double get_current_rsi() const { return current_rsi_; }
    
private:
    int ma_period_;
    double rsi_threshold_;
    
    std::vector<double> price_history_;
    double current_ma_;
    double current_rsi_;
    
    // Calculate moving average
    double calculate_ma(const std::vector<double>& prices, int period) const;
    
    // Calculate RSI
    double calculate_rsi(const std::vector<double>& prices, int period) const;
    
    // Check if sufficient data available
    bool has_sufficient_data() const { return price_history_.size() >= static_cast<size_t>(ma_period_ + 14); }
};