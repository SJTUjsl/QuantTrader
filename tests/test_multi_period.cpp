#include "../backend/data/timeframe_manager.h"
#include "../backend/core/utils/logger.h"
#include <iostream>
#include <vector>

int main() {
    Logger::get_instance().init("logs/multi_period.log");
    
    Logger::get_instance().info("Starting multi-period linkage test...");
    
    // Create timeframe manager
    TimeFrameManager tf_manager;
    
    // Register timeframes for AAPL
    tf_manager.register_timeframe("AAPL", TimeFrame::SECOND_1);
    tf_manager.register_timeframe("AAPL", TimeFrame::MINUTE_1);
    
    // Generate test tick data
    std::vector<MarketData> ticks;
    double base_price = 100.0;
    
    // Generate 180 ticks (3 minutes worth of 1-second data)
    for (int i = 0; i < 180; ++i) {
        double price = base_price + (i * 0.1) + (rand() % 10 - 5) * 0.05;
        std::string timestamp = "2024-01-01 09:30:" + 
                               ((i < 10) ? "0" + std::to_string(i) : std::to_string(i));
        
        ticks.emplace_back("AAPL", timestamp, 
                          price - 0.1, price + 0.1, price - 0.05, price, 1000 + i * 10);
    }
    
    std::cout << "Processing " << ticks.size() << " ticks for multi-period test..." << std::endl;
    
    // Process ticks
    for (size_t i = 0; i < ticks.size(); ++i) {
        tf_manager.process_tick(ticks[i]);
        
        // Check for new 1-minute candles
        if (tf_manager.has_new_candle("AAPL", TimeFrame::MINUTE_1)) {
            auto candle = tf_manager.get_latest_candle("AAPL", TimeFrame::MINUTE_1);
            std::cout << "New 1-minute candle: " << candle.timestamp 
                      << " O:" << candle.open << " H:" << candle.high
                      << " L:" << candle.low << " C:" << candle.close
                      << " V:" << candle.volume << std::endl;
        }
    }
    
    // Get final candle counts
    auto second1_candles = tf_manager.get_candles("AAPL", TimeFrame::SECOND_1);
    auto minute1_candles = tf_manager.get_candles("AAPL", TimeFrame::MINUTE_1);
    
    std::cout << "\nResults:" << std::endl;
    std::cout << "1-second candles: " << second1_candles.size() << std::endl;
    std::cout << "1-minute candles: " << minute1_candles.size() << std::endl;
    
    // Display some 1-minute candles
    std::cout << "\nLast 3 1-minute candles:" << std::endl;
    for (int i = std::max(0, static_cast<int>(minute1_candles.size()) - 3); 
         i < minute1_candles.size(); ++i) {
        const auto& candle = minute1_candles[i];
        std::cout << "  " << candle.timestamp << ": O=" << candle.open
                  << " H=" << candle.high << " L=" << candle.low
                  << " C=" << candle.close << std::endl;
    }
    
    Logger::get_instance().info("Multi-period linkage test completed");
    
    return 0;
}