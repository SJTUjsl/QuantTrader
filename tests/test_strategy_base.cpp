#include "../backend/strategy/dummy_strategy.h"
#include "../backend/strategy/strategy_base.h"
#include "../backend/core/utils/logger.h"
#include "../backend/data/data_adapter.h"
#include "../backend/data/market_data_feed.h"
#include <iostream>

int main() {
    Logger::get_instance().init("logs/strategy_base.log");
    
    Logger::get_instance().info("Starting strategy base test...");
    
    // Create dummy strategy
    DummyStrategy strategy;
    strategy.init();
    
    // Mock market data
    MarketData test_data("AAPL", "2024-01-01 10:00:00", 150.0, 152.0, 149.5, 151.5, 1000000);
    
    // Update strategy data
    strategy.update_market_data(test_data);
    
    // Generate signal
    Signal signal = strategy.generate_signal();
    
    // Output results
    std::cout << "Strategy test results:" << std::endl;
    std::cout << "Signal type: ";
    switch (signal.type) {
        case SignalType::BUY:
            std::cout << "BUY";
            break;
        case SignalType::SELL:
            std::cout << "SELL";
            break;
        case SignalType::HOLD:
            std::cout << "HOLD";
            break;
    }
    std::cout << std::endl;
    std::cout << "Symbol: " << signal.symbol << std::endl;
    std::cout << "Price: " << signal.price << std::endl;
    std::cout << "Quantity: " << signal.quantity << std::endl;
    std::cout << "Timestamp: " << signal.timestamp << std::endl;
    
    // Test position update
    Position position("AAPL", 100, 150.0, 151.5);
    strategy.update_position(position);
    
    // Generate signal again
    Signal signal2 = strategy.generate_signal();
    
    std::cout << "\nAfter position update:" << std::endl;
    std::cout << "Second signal type: ";
    switch (signal2.type) {
        case SignalType::BUY:
            std::cout << "BUY";
            break;
        case SignalType::SELL:
            std::cout << "SELL";
            break;
        case SignalType::HOLD:
            std::cout << "HOLD";
            break;
    }
    std::cout << std::endl;
    
    strategy.reset();
    
    Logger::get_instance().info("Strategy base test completed");
    
    return 0;
}