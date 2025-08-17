#include "market_data_feed.h"
#include "data_adapter.h"
#include "../core/utils/logger.h"

// 使用简单字符串代替json库
class SimpleJson {
public:
    static std::string market_data_to_json(const MarketData& data) {
        return "{\"symbol\":\"" + data.symbol + "\"," +
               "\"timestamp\":\"" + data.timestamp + "\"," +
               "\"open\":" + std::to_string(data.open) + "," +
               "\"high\":" + std::to_string(data.high) + "," +
               "\"low\":" + std::to_string(data.low) + "," +
               "\"close\":" + std::to_string(data.close) + "," +
               "\"volume\":" + std::to_string(data.volume) + "}";
    }
};

MarketDataFeed::MarketDataFeed() : running_(false) {
    adapter_ = std::make_unique<MockDataAdapter>();
}

MarketDataFeed::~MarketDataFeed() {
    stop();
}

void MarketDataFeed::subscribe(const std::string& symbol) {
    current_symbol_ = symbol;
    adapter_->subscribe(symbol);
    Logger::get_instance().info("MarketDataFeed subscribed to: " + symbol);
}

void MarketDataFeed::unsubscribe(const std::string& symbol) {
    if (current_symbol_ == symbol) {
        adapter_->unsubscribe(symbol);
        current_symbol_.clear();
        Logger::get_instance().info("MarketDataFeed unsubscribed from: " + symbol);
    }
}

void MarketDataFeed::start() {
    if (!running_) {
        running_ = true;
        
        adapter_->set_callback([this](const MarketData& data) {
            on_data_received(data);
        });
        
        adapter_->start();
        Logger::get_instance().info("MarketDataFeed started");
    }
}

void MarketDataFeed::stop() {
    if (running_) {
        running_ = false;
        adapter_->stop();
        Logger::get_instance().info("MarketDataFeed stopped");
    }
}

void MarketDataFeed::on_data_received(const MarketData& data) {
    // 记录接收到的数据
    Logger::get_instance().info("MarketDataFeed received: " + data.symbol + 
                               " @ " + std::to_string(data.close) + 
                               " Volume: " + std::to_string(data.volume));
    
    // 通过消息总线分发
    publish_to_bus(data);
}

void MarketDataFeed::publish_to_bus(const MarketData& data) {
    std::string json_data = SimpleJson::market_data_to_json(data);
    MessageBus::get_instance().publish("market_data", json_data);
    
    Logger::get_instance().info("Published to message bus: " + data.symbol);
}