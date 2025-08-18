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
    
    // Test data: simulated price series
    std::vector<MarketData> test_data;
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:30:00", 100.0, 101.0, 99.0, 100.0, 1000000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:31:00", 100.0, 102.0, 99.5, 101.5, 1200000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:32:00", 101.5, 103.0, 101.0, 102.5, 900000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:33:00", 102.5, 104.0, 102.0, 103.5, 1100000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:34:00", 103.5, 105.0, 103.0, 104.5, 1300000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:35:00", 104.5, 106.0, 104.0, 105.5, 1000000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:36:00", 105.5, 107.0, 105.0, 106.5, 1400000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:37:00", 106.5, 108.0, 106.0, 107.5, 1200000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:38:00", 107.5, 109.0, 107.0, 108.5, 1500000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:39:00", 108.5, 110.0, 108.0, 109.5, 1100000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:40:00", 109.5, 111.0, 109.0, 110.5, 1300000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:41:00", 110.5, 112.0, 110.0, 111.5, 1000000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:42:00", 111.5, 113.0, 111.0, 112.5, 1200000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:43:00", 112.5, 114.0, 112.0, 113.5, 1400000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:44:00", 113.5, 115.0, 113.0, 114.5, 1100000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:45:00", 114.5, 116.0, 114.0, 115.5, 1300000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:46:00", 115.5, 117.0, 115.0, 116.5, 1500000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:47:00", 116.5, 118.0, 116.0, 117.5, 1200000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:48:00", 117.5, 119.0, 117.0, 118.5, 1000000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:49:00", 118.5, 120.0, 118.0, 119.5, 1400000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:50:00", 119.5, 121.0, 119.0, 120.5, 1100000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:51:00", 120.5, 122.0, 120.0, 121.5, 1300000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:52:00", 121.5, 123.0, 121.0, 122.5, 1500000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:53:00", 122.5, 124.0, 122.0, 123.5, 1200000));
    test_data.push_back(MarketData("AAPL", "2024-01-01 09:54:00", 123.5, 125.0, 123.0, 124.5, 1400000));
    
    std::cout << "Testing MA RSI Strategy with " << test_data.size() << " data points..." << std::endl;
    
    // Simulate data flow
    for (size_t i = 0; i < test_data.size(); ++i) {
        strategy.update_market_data(test_data[i]);
        strategy.on_bar(test_data[i]);
        
        // Generate signal every 5 data points
        if ((i + 1) % 5 == 0 || i == test_data.size() - 1) {
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