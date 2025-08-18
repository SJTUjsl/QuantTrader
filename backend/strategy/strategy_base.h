#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../common/market_data.h"

enum class SignalType {
    BUY,
    SELL,
    HOLD
};

struct Signal {
    SignalType type;
    std::string symbol;
    double price;
    int quantity;
    std::string timestamp;
    
    Signal() : type(SignalType::HOLD), price(0), quantity(0) {}
    Signal(SignalType t, const std::string& s, double p, int q, const std::string& ts)
        : type(t), symbol(s), price(p), quantity(q), timestamp(ts) {}
};

struct Position {
    std::string symbol;
    int quantity;
    double average_price;
    double current_price;
    
    Position() : quantity(0), average_price(0), current_price(0) {}
    Position(const std::string& s, int q, double avg, double cur)
        : symbol(s), quantity(q), average_price(avg), current_price(cur) {}
};

class StrategyBase {
public:
    StrategyBase(const std::string& name);
    virtual ~StrategyBase() = default;
    
    virtual void on_bar(const MarketData& data) = 0;
    virtual Signal generate_signal() = 0;
    
    void update_market_data(const MarketData& data);
    void update_position(const Position& position);
    
    virtual void init() = 0;
    virtual void reset() = 0;
    
    const std::string& get_name() const { return name_; }
    bool is_active() const { return active_; }
    void set_active(bool active) { active_ = active; }
    
protected:
    std::string name_;
    bool active_;
    
    MarketData current_data_;
    Position current_position_;
    
    double calculate_returns() const;
    bool has_position() const { return current_position_.quantity > 0; }
};