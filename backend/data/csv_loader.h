#pragma once

#include <string>
#include <vector>
#include <fstream>

struct OHLCV {
    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
    int64_t volume;
    
    OHLCV() : open(0), high(0), low(0), close(0), volume(0) {}
    OHLCV(const std::string& ts, double o, double h, double l, double c, int64_t v)
        : timestamp(ts), open(o), high(h), low(l), close(c), volume(v) {}
};

class CSVLoader {
public:
    CSVLoader() = default;
    ~CSVLoader() = default;
    
    std::vector<OHLCV> load_from_file(const std::string& file_path);
    
private:
    OHLCV parse_line(const std::string& line);
    std::vector<std::string> split_line(const std::string& line, char delimiter = ',');
};