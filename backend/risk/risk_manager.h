#pragma once

#include "../execution/order_manager.h"
#include "../core/utils/logger.h"
#include <string>
#include <vector>

struct RiskCheckResult {
    bool is_allowed;
    std::string rejection_reason;
    
    RiskCheckResult(bool allowed = true, const std::string& reason = "")
        : is_allowed(allowed), rejection_reason(reason) {}
};

class RiskManager {
public:
    struct RiskConfig {
        double max_single_position_pct = 0.1;    // Max single position 10% of capital
        double max_daily_loss_pct = 0.05;        // Max daily loss 5%
        double max_drawdown_pct = 0.15;          // Max drawdown 15%
        int max_position_count = 10;             // Max position count
        bool block_st_stocks = true;             // Block ST stocks
        
        RiskConfig() = default;
    };
    
    RiskManager(const RiskConfig& config = RiskConfig());
    ~RiskManager() = default;
    
    // Check if order passes all risk rules
    RiskCheckResult check_order(const std::shared_ptr<Order>& order);
    
    // Check specific risk rules
    RiskCheckResult check_st_stock(const std::string& symbol);
    RiskCheckResult check_position_limit(const std::string& symbol, int quantity, double price);
    RiskCheckResult check_daily_loss_limit(double potential_loss);
    RiskCheckResult check_drawdown_limit(double current_drawdown);
    
    // Update risk state
    void update_daily_pnl(double pnl);
    void update_portfolio_value(double value);
    
    // Get current risk status
    double get_daily_pnl() const { return daily_pnl_; }
    double get_max_drawdown() const { return max_drawdown_; }
    int get_position_count() const { return position_count_; }
    
private:
    RiskConfig config_;
    double initial_capital_ = 100000.0;  // 初始资金
    double daily_pnl_ = 0.0;
    double max_drawdown_ = 0.0;
    double peak_portfolio_value_ = 100000.0;
    int position_count_ = 0;
    
    // ST股票列表（示例）
    std::vector<std::string> st_stocks_ = {"ST_STOCK_1", "ST_STOCK_2", "ST_STOCK_3"};
    
    bool is_st_stock(const std::string& symbol) const;
};