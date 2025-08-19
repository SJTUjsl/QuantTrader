#include "order_manager.h"

std::string Order::to_json() const {
    std::ostringstream json;
    json << "{\n";
    json << "  \"id\": \"" << id_ << "\",\n";
    json << "  \"symbol\": \"" << symbol_ << "\",\n";
    json << "  \"side\": \"" << (side_ == OrderSide::BUY ? "BUY" : "SELL") << "\",\n";
    json << "  \"type\": \"" << (type_ == OrderType::MARKET ? "MARKET" : "LIMIT") << "\",\n";
    json << "  \"quantity\": " << quantity_ << ",\n";
    json << "  \"price\": " << std::fixed << std::setprecision(2) << price_ << "\n";
    json << "}";
    return json.str();
}

OrderManager::OrderManager() {
    Logger::get_instance().info("OrderManager initialized");
}

std::shared_ptr<Order> OrderManager::process_signal(const Signal& signal, const std::string& strategy_id) {
    if (signal.type == SignalType::HOLD) {
        return nullptr;
    }
    
    auto order = create_order_from_signal(signal, strategy_id);
    orders_.push_back(order);
    
    Logger::get_instance().info("Order created from signal: " + order->to_json());
    return order;
}

std::shared_ptr<Order> OrderManager::create_order_from_signal(const Signal& signal, const std::string& strategy_id) {
    OrderSide side = (signal.type == SignalType::BUY) ? OrderSide::BUY : OrderSide::SELL;
    return std::make_shared<Order>(signal.symbol, side, OrderType::MARKET, signal.quantity, signal.price);
}

std::vector<std::shared_ptr<Order>> OrderManager::get_active_orders() const {
    std::vector<std::shared_ptr<Order>> active_orders;
    for (const auto& order : orders_) {
        if (order->get_status() == OrderStatus::PENDING || order->get_status() == OrderStatus::SUBMITTED) {
            active_orders.push_back(order);
        }
    }
    return active_orders;
}

std::shared_ptr<Order> OrderManager::get_order(const std::string& order_id) const {
    for (const auto& order : orders_) {
        if (order->get_id() == order_id) {
            return order;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Order>> OrderManager::get_orders_for_symbol(const std::string& symbol) const {
    std::vector<std::shared_ptr<Order>> symbol_orders;
    for (const auto& order : orders_) {
        if (order->get_symbol() == symbol) {
            symbol_orders.push_back(order);
        }
    }
    return symbol_orders;
}

bool OrderManager::cancel_order(const std::string& order_id) {
    for (auto& order : orders_) {
        if (order->get_id() == order_id) {
            Logger::get_instance().info("Order cancelled: " + order_id);
            return true;
        }
    }
    return false;
}