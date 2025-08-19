#include "../backend/execution/order_manager.h"
#include "../backend/strategy/ma_rsi_strategy.h"
#include "../backend/core/utils/logger.h"
#include <iostream>

int main() {
    Logger::get_instance().init("logs/order_manager.log");
    
    std::cout << "Testing Order Manager..." << std::endl;
    
    OrderManager order_manager;
    
    // Create test signals
    Signal buy_signal(SignalType::BUY, "AAPL", 150.0, 100, "2024-01-01 09:30:00");
    Signal sell_signal(SignalType::SELL, "AAPL", 155.0, 50, "2024-01-01 09:31:00");
    Signal hold_signal(SignalType::HOLD, "AAPL", 152.0, 0, "2024-01-01 09:32:00");
    
    // Test order creation from signals
    auto buy_order = order_manager.process_signal(buy_signal, "MA_RSI_Strategy");
    auto sell_order = order_manager.process_signal(sell_signal, "MA_RSI_Strategy");
    auto hold_order = order_manager.process_signal(hold_signal, "MA_RSI_Strategy");
    
    // Verify results
    std::cout << "\nResults:" << std::endl;
    std::cout << "Buy order created: " << (buy_order ? "YES" : "NO") << std::endl;
    std::cout << "Sell order created: " << (sell_order ? "YES" : "NO") << std::endl;
    std::cout << "Hold order created: " << (hold_order ? "YES" : "NO") << std::endl;
    
    if (buy_order) {
        std::cout << "Buy order JSON:\n" << buy_order->to_json() << std::endl;
    }
    
    std::cout << "\nOrder Manager test completed successfully!" << std::endl;
    
    return 0;
}