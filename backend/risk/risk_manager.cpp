#include "risk_manager.h"

RiskManager::RiskManager(const RiskConfig& config) 
    : config_(config), initial_capital_(100000.0), daily_pnl_(0.0), 
      max_drawdown_(0.0), peak_portfolio_value_(100000.0), position_count_(0) {
    Logger::get_instance().info("RiskManager initialized");
}

RiskCheckResult RiskManager::check_order(const std::shared_ptr<Order>& order) {
    // Check ST stock restriction
    auto st_result = check_st_stock(order->get_symbol());
    if (!st_result.is_allowed) return st_result;
    
    // Check position limit
    auto position_result = check_position_limit(order->get_symbol(), 
                                                 order->get_quantity(), 150.0);
    if (!position_result.is_allowed) return position_result;
    
    // Additional checks for BUY orders
    if (order->get_side() == OrderSide::BUY) {
        // Check daily loss limit (simplified)
        double order_value = order->get_quantity() * 150.0;
        double potential_loss = order_value * 0.1; // Assume 10% max loss per position
        
        auto loss_result = check_daily_loss_limit(potential_loss);
        if (!loss_result.is_allowed) return loss_result;
    }
    
    Logger::get_instance().info("Risk check passed for order: " + order->get_symbol());
    return RiskCheckResult(true, "");
}

RiskCheckResult RiskManager::check_st_stock(const std::string& symbol) {
    if (config_.block_st_stocks && is_st_stock(symbol)) {
        return RiskCheckResult(false, "ST股票禁止交易: " + symbol);
    }
    return RiskCheckResult(true, "");
}

RiskCheckResult RiskManager::check_position_limit(const std::string& symbol, 
                                                int quantity, double price) {
    double position_value = quantity * price;
    double max_single_position = initial_capital_ * config_.max_single_position_pct;
    
    if (position_value > max_single_position) {
        return RiskCheckResult(false, 
            "单笔仓位超限: " + std::to_string(position_value) + " > " + 
            std::to_string(max_single_position));
    }
    
    if (position_count_ >= config_.max_position_count) {
        return RiskCheckResult(false, 
            "持仓数量超限: " + std::to_string(position_count_) + " >= " + 
            std::to_string(config_.max_position_count));
    }
    
    return RiskCheckResult(true, "");
}

RiskCheckResult RiskManager::check_daily_loss_limit(double potential_loss) {
    double max_daily_loss = initial_capital_ * config_.max_daily_loss_pct;
    
    if (daily_pnl_ - potential_loss < -max_daily_loss) {
        return RiskCheckResult(false, 
            "日亏损超限: " + std::to_string(daily_pnl_ - potential_loss) + " < -" + 
            std::to_string(max_daily_loss));
    }
    
    return RiskCheckResult(true, "");
}

RiskCheckResult RiskManager::check_drawdown_limit(double current_drawdown) {
    if (current_drawdown > config_.max_drawdown_pct) {
        return RiskCheckResult(false, 
            "最大回撤超限: " + std::to_string(current_drawdown * 100) + "% > " + 
            std::to_string(config_.max_drawdown_pct * 100) + "%");
    }
    return RiskCheckResult(true, "");
}

void RiskManager::update_daily_pnl(double pnl) {
    daily_pnl_ += pnl;
    Logger::get_instance().info("Daily PnL updated: " + std::to_string(daily_pnl_));
}

void RiskManager::update_portfolio_value(double value) {
    if (value > peak_portfolio_value_) {
        peak_portfolio_value_ = value;
    }
    
    double drawdown = (peak_portfolio_value_ - value) / peak_portfolio_value_;
    if (drawdown > max_drawdown_) {
        max_drawdown_ = drawdown;
    }
    
    Logger::get_instance().info("Portfolio value updated: " + std::to_string(value) + 
                              ", max drawdown: " + std::to_string(max_drawdown_ * 100) + "%");
}

bool RiskManager::is_st_stock(const std::string& symbol) const {
    // Simple ST stock detection - in real implementation, this would come from market data
    return symbol.find("ST") != std::string::npos || 
           symbol.find("*") != std::string::npos;
}