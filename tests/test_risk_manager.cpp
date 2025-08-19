#include "../backend/risk/risk_manager.h"
#include "../backend/execution/order_manager.h"
#include "../backend/core/utils/logger.h"
#include <iostream>
#include <windows.h>

int main() {
    // 设置控制台为UTF-8编码
    SetConsoleOutputCP(65001);
    Logger::get_instance().init("logs/risk_manager.log");
    
    std::cout << "Testing Risk Manager..." << std::endl;
    
    // Create risk manager with default config
    RiskManager::RiskConfig config;
    config.max_single_position_pct = 0.1;  // 10%
    config.max_daily_loss_pct = 0.05;      // 5%
    config.max_drawdown_pct = 0.15;        // 15%
    config.block_st_stocks = true;
    
    RiskManager risk_manager(config);
    
    // Create order manager
    OrderManager order_manager;
    
    // Test 1: Normal order
    std::cout << "\nTest 1: Normal order" << std::endl;
    Signal normal_signal(SignalType::BUY, "AAPL", 150.0, 100, "2024-01-01 09:30:00");
    auto normal_order = order_manager.process_signal(normal_signal, "MA_RSI_Strategy");
    
    if (normal_order) {
        auto result = risk_manager.check_order(normal_order);
        std::cout << "Normal order result: " << (result.is_allowed ? "ALLOWED" : "REJECTED") << std::endl;
        if (!result.is_allowed) {
            std::cout << "Reason: " << result.rejection_reason << std::endl;
        }
    }
    
    // Test 2: ST stock
    std::cout << "\nTest 2: ST stock" << std::endl;
    Signal st_signal(SignalType::BUY, "ST_STOCK_1", 10.0, 1000, "2024-01-01 09:31:00");
    auto st_order = order_manager.process_signal(st_signal, "MA_RSI_Strategy");
    
    if (st_order) {
        auto result = risk_manager.check_order(st_order);
        std::cout << "ST stock result: " << (result.is_allowed ? "ALLOWED" : "REJECTED") << std::endl;
        if (!result.is_allowed) {
            std::cout << "Reason: " << result.rejection_reason << std::endl;
        }
    }
    
    // Test 3: Position limit
    std::cout << "\nTest 3: Position limit" << std::endl;
    Signal large_signal(SignalType::BUY, "AAPL", 150.0, 10000, "2024-01-01 09:32:00");
    auto large_order = order_manager.process_signal(large_signal, "MA_RSI_Strategy");
    
    if (large_order) {
        auto result = risk_manager.check_order(large_order);
        std::cout << "Large position result: " << (result.is_allowed ? "ALLOWED" : "REJECTED") << std::endl;
        if (!result.is_allowed) {
            std::cout << "Reason: " << result.rejection_reason << std::endl;
        }
    }
    
    // Test 4: Update portfolio and check drawdown
    std::cout << "\nTest 4: Drawdown check" << std::endl;
    risk_manager.update_portfolio_value(95000);  // 5% drawdown
    auto drawdown_result = risk_manager.check_drawdown_limit(0.05);
    std::cout << "Drawdown check: " << (drawdown_result.is_allowed ? "ALLOWED" : "REJECTED") << std::endl;
    
    std::cout << "\nRisk Manager test completed!" << std::endl;
    
    return 0;
}