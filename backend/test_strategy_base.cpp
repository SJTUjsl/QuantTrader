#include "strategy/dummy_strategy.h"
#include "strategy/strategy_base.h"
#include "core/utils/logger.h"
#include "data/data_adapter.h"
#include <iostream>

int main() {
    Logger::get_instance().init("logs/strategy_base.log");
    
    Logger::get_instance().info("Starting strategy base test...");
    
    // 创建dummy策略
    DummyStrategy strategy;
    strategy.init();
    
    // 模拟行情数据
    MarketData test_data("AAPL", "2024-01-01 10:00:00", 150.0, 152.0, 149.5, 151.5, 1000000);
    
    // 更新策略数据
    strategy.update_market_data(test_data);
    
    // 生成信号
    Signal signal = strategy.generate_signal();
    
    // 输出结果
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
    std::cout << "\nSymbol: " << signal.symbol << std::endl;
    std::cout << "Price: " << signal.price << std::endl;
    std::cout << "Quantity: " << signal.quantity << std::endl;
    std::cout << "Timestamp: " << signal.timestamp << std::endl;
    
    // 测试持仓更新
    Position position("AAPL", 100, 150.0, 151.5);
    strategy.update_position(position);
    
    // 再次生成信号
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