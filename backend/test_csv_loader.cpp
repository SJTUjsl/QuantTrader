#include "data/csv_loader.h"
#include "core/utils/logger.h"
#include <iostream>

int main() {
    Logger::get_instance().init("logs/csv_loader.log");
    
    CSVLoader loader;
    
    Logger::get_instance().info("Starting CSV loader test...");
    
    std::vector<OHLCV> data = loader.load_from_file("backend/data/storage/sample_data.csv");
    
    if (data.empty()) {
        Logger::get_instance().error("No data loaded from CSV");
        return 1;
    }
    
    Logger::get_instance().info("Loaded " + std::to_string(data.size()) + " records");
    
    // 输出第一条记录作为验证
    if (!data.empty()) {
        const OHLCV& first = data[0];
        std::cout << "First record:"
                  << "\n  Timestamp: " << first.timestamp
                  << "\n  Open: " << first.open
                  << "\n  High: " << first.high
                  << "\n  Low: " << first.low
                  << "\n  Close: " << first.close
                  << "\n  Volume: " << first.volume
                  << std::endl;
    }
    
    Logger::get_instance().info("CSV loader test completed");
    
    return 0;
}