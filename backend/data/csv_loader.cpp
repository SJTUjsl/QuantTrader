#include "csv_loader.h"
#include "../core/utils/logger.h"
#include <sstream>
#include <iostream>

std::vector<OHLCV> CSVLoader::load_from_file(const std::string& file_path) {
    std::vector<OHLCV> data;
    std::ifstream file(file_path);
    
    if (!file.is_open()) {
        Logger::get_instance().error("Failed to open CSV file: " + file_path);
        return data;
    }
    
    Logger::get_instance().info("Loading CSV file: " + file_path);
    
    std::string line;
    bool is_first_line = true;
    
    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;
        
        // Skip header line
        if (is_first_line) {
            is_first_line = false;
            // Check if this is a header line by looking for non-numeric content
            if (line.find("timestamp") != std::string::npos || 
                line.find("open") != std::string::npos ||
                line.find("close") != std::string::npos) {
                continue;
            }
        }
        
        try {
            OHLCV bar = parse_line(line);
            data.push_back(bar);
        } catch (const std::exception& e) {
            Logger::get_instance().warn("Failed to parse line: " + line + " Error: " + e.what());
        }
    }
    
    Logger::get_instance().info("Loaded " + std::to_string(data.size()) + " records from CSV");
    return data;
}

OHLCV CSVLoader::parse_line(const std::string& line) {
    std::vector<std::string> fields = split_line(line);
    
    if (fields.size() < 6) {
        throw std::runtime_error("Insufficient fields in CSV line");
    }
    
    OHLCV bar;
    bar.timestamp = fields[0];
    
    try {
        bar.open = std::stod(fields[1]);
        bar.high = std::stod(fields[2]);
        bar.low = std::stod(fields[3]);
        bar.close = std::stod(fields[4]);
        bar.volume = std::stoll(fields[5]);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Invalid numeric format: " + std::string(e.what()));
    }
    
    return bar;
}

std::vector<std::string> CSVLoader::split_line(const std::string& line, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string item;
    
    while (std::getline(ss, item, delimiter)) {
        // Remove quotes and trim whitespace
        if (!item.empty() && item.front() == '"') item.erase(0, 1);
        if (!item.empty() && item.back() == '"') item.pop_back();
        result.push_back(item);
    }
    
    return result;
}