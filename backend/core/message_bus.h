#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>

class MessageBus {
public:
    using MessageHandler = std::function<void(const std::string& topic, const std::string& data)>;
    
    static MessageBus& get_instance();
    
    void subscribe(const std::string& topic, MessageHandler handler);
    void unsubscribe(const std::string& topic, MessageHandler handler);
    void publish(const std::string& topic, const std::string& data);
    
private:
    MessageBus() = default;
    ~MessageBus() = default;
    
    MessageBus(const MessageBus&) = delete;
    MessageBus& operator=(const MessageBus&) = delete;
    
    std::mutex mutex_;
    std::unordered_map<std::string, std::vector<MessageHandler>> subscribers_;
};