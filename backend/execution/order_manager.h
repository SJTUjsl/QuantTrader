#pragma once

#include "../strategy/strategy_base.h"
#include "../core/utils/logger.h"
#include <vector>
#include <memory>
#include <random>
#include <sstream>
#include <iomanip>

enum class OrderStatus {
    PENDING,
    SUBMITTED,
    FILLED,
    PARTIALLY_FILLED,
    CANCELLED,
    REJECTED,
    EXPIRED
};

enum class OrderType {
    MARKET,
    LIMIT
};

enum class OrderSide {
    BUY,
    SELL
};

class Order {
public:
    Order(const std::string& symbol, OrderSide side, OrderType type, 
          int quantity, double price = 0.0, const std::string& strategy_id = "")
        : id_(generate_id()), symbol_(symbol), side_(side), type_(type),
          quantity_(quantity), filled_quantity_(0), price_(price), status_(OrderStatus::PENDING) {}
    
    std::string get_id() const { return id_; }
    std::string get_symbol() const { return symbol_; }
    OrderSide get_side() const { return side_; }
    int get_quantity() const { return quantity_; }
    OrderStatus get_status() const { return status_; }
    std::string to_json() const;
    
private:
    std::string id_, symbol_;
    OrderSide side_;
    OrderType type_;
    int quantity_, filled_quantity_;
    double price_;
    OrderStatus status_;
    
    static std::string generate_id() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dis(1000, 9999);
        return "ORD" + std::to_string(dis(gen));
    }
};

class OrderManager {
public:
    OrderManager();
    ~OrderManager() = default;
    
    // Process signal from strategy and create order
    std::shared_ptr<Order> process_signal(const Signal& signal, const std::string& strategy_id);
    
    // Get all active orders
    std::vector<std::shared_ptr<Order>> get_active_orders() const;
    
    // Get order by ID
    std::shared_ptr<Order> get_order(const std::string& order_id) const;
    
    // Get orders for symbol
    std::vector<std::shared_ptr<Order>> get_orders_for_symbol(const std::string& symbol) const;
    
    // Cancel order
    bool cancel_order(const std::string& order_id);
    
private:
    std::vector<std::shared_ptr<Order>> orders_;
    
    // Create order from signal
    std::shared_ptr<Order> create_order_from_signal(const Signal& signal, const std::string& strategy_id);
};