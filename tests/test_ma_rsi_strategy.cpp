#include "../backend/strategy/ma_rsi_strategy.h"
#include "../backend/core/utils/logger.h"
#include <iostream>
#include <vector>

int main() {
    Logger::get_instance().init("logs/ma_rsi_strategy.log");
    
    Logger::get_instance().info("Starting MA RSI Strategy test...");
    
    // Create strategy instance with MA period 20 and RSI threshold 30
    MA_RSI_Strategy strategy(20, 30.0);
    strategy.init();
    
    // Test data: simulated price series with sufficient data for indicators
    std::vector<MarketData> test_data;
    
    // Generate 50 data points for proper indicator calculation
    double base_price = 100.0;
    for (int i = 0; i < 50; ++i) {
        // Create trending data with some volatility
        double trend = 0.5 * i; // Upward trend
        double volatility = (rand() % 20 - 10) * 0.1; // Random volatility
        double close = base_price + trend + volatility;
        
        std::string timestamp = "2024-01-01 09:" + std::to_string(30 + i / 60) + ":" + std::to_string(i % 60);
        if ((i % 60) < 10) timestamp += "0";
        timestamp = "2024-01-01 09:" + std::to_string(30 + i / 60) + ":" + std::to_string(i % 60);
        if ((i % 60) < 10) timestamp.insert(timestamp.length() - 1, "0");
        
        test_data.push_back(MarketData("AAPL", 
                                      "2024-01-01 09:" + std::to_string(30 + i / 60) + ":" + 
                                      ((i % 60 < 10) ? "0" + std::to_string(i % 60) : std::to_string(i % 60)),
                                      close - 1.0, close + 1.0, close - 0.5, close, 1000000 + i * 10000));
    }
    
    std::cout << "Testing MA RSI Strategy with " << test_data.size() << " data points..." << std::endl;
    
    // Simulate data flow
    for (size_t i = 0; i < test_data.size(); ++i) {
        strategy.update_market_data(test_data[i]);
        strategy.on_bar(test_data[i]);
        
        // Generate signal for last 10 data points when we have sufficient data
        if (i >= 34 && (i + 1) % 3 == 0) {
            Signal signal = strategy.generate_signal();
            
            std::cout << "Signal " << (i + 1) << ": ";
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
            std::cout << " at " << signal.price << std::endl;
        }
    }
    
    std::cout << "Final MA: " << strategy.get_current_ma() << std::endl;
    std::cout << "Final RSI: " << strategy.get_current_rsi() << std::endl;
    
    Logger::get_instance().info("MA RSI Strategy test completed");
    
    return 0;
}