#include "../backend/data/csv_loader.h"
#include "../backend/core/utils/logger.h"
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
    
    // Output first record for validation
    if (!data.empty()) {
        const OHLCV& first = data[0];
        std::cout << "First record:" << std::endl;
        std::cout << "  Timestamp: " << first.timestamp << std::endl;
        std::cout << "  Open: " << first.open << std::endl;
        std::cout << "  High: " << first.high << std::endl;
        std::cout << "  Low: " << first.low << std::endl;
        std::cout << "  Close: " << first.close << std::endl;
        std::cout << "  Volume: " << first.volume << std::endl;
    }
    
    Logger::get_instance().info("CSV loader test completed");
    
    return 0;
}