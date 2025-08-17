#include "message_bus.h"
#include "utils/logger.h"
#include <algorithm>

MessageBus& MessageBus::get_instance() {
    static MessageBus instance;
    return instance;
}

void MessageBus::subscribe(const std::string& topic, MessageHandler handler) {
    std::lock_guard<std::mutex> lock(mutex_);
    subscribers_[topic].push_back(handler);
    Logger::get_instance().info("Subscribed to topic: " + topic);
}

void MessageBus::unsubscribe(const std::string& topic, MessageHandler handler) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = subscribers_.find(topic);
    if (it != subscribers_.end()) {
        auto& handlers = it->second;
        handlers.erase(
            std::remove_if(handlers.begin(), handlers.end(),
                [&](const MessageHandler& h) { return h.target_type() == handler.target_type(); }),
            handlers.end()
        );
        Logger::get_instance().info("Unsubscribed from topic: " + topic);
    }
}

void MessageBus::publish(const std::string& topic, const std::string& data) {
    std::vector<MessageHandler> handlers_to_call;
    
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = subscribers_.find(topic);
        if (it != subscribers_.end()) {
            handlers_to_call = it->second;
        }
    }
    
    Logger::get_instance().info("Publishing to topic: " + topic + " with data: " + data);
    
    for (const auto& handler : handlers_to_call) {
        try {
            handler(topic, data);
        } catch (const std::exception& e) {
            Logger::get_instance().error("Message handler failed: " + std::string(e.what()));
        }
    }
}