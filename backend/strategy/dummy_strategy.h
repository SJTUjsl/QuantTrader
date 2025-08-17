#pragma once

#include "strategy_base.h"

class DummyStrategy : public StrategyBase {
public:
    DummyStrategy();
    ~DummyStrategy() override = default;
    
    void init() override;
    void reset() override;
    void on_bar(const MarketData& data) override;
    Signal generate_signal() override;
    
private:
    int counter_;
    Signal last_signal_;
};