#pragma once

#include <functional>
#include <chrono>
#include <map>
#include <thread>
#include <atomic>
#include <mutex>

class EventLoop {
public:
    using Callback = std::function<void()>;
    using TimerId = int64_t;
    
    EventLoop();
    ~EventLoop();
    
    void start();
    void stop();
    
    TimerId register_timer(std::chrono::milliseconds interval, Callback callback);
    void cancel_timer(TimerId timer_id);
    
    void post_event(Callback callback);
    
private:
    void run();
    std::chrono::steady_clock::time_point process_timers();
    
    struct Timer {
        std::chrono::steady_clock::time_point next_fire;
        std::chrono::milliseconds interval;
        Callback callback;
        bool active;
    };
    
    std::atomic<bool> running_;
    std::thread worker_thread_;
    
    std::mutex mutex_;
    std::map<TimerId, Timer> timers_;
    TimerId next_timer_id_;
    
    std::vector<Callback> pending_events_;
};