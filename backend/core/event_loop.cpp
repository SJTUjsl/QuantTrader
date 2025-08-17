#include "event_loop.h"
#include "utils/logger.h"
#include <algorithm>

EventLoop::EventLoop() : running_(false), next_timer_id_(1) {}

EventLoop::~EventLoop() {
    stop();
}

void EventLoop::start() {
    if (!running_.exchange(true)) {
        worker_thread_ = std::thread(&EventLoop::run, this);
        Logger::get_instance().info("Event loop started");
    }
}

void EventLoop::stop() {
    if (running_.exchange(false)) {
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }
        Logger::get_instance().info("Event loop stopped");
    }
}

EventLoop::TimerId EventLoop::register_timer(std::chrono::milliseconds interval, Callback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    TimerId timer_id = next_timer_id_++;
    Timer timer;
    timer.next_fire = std::chrono::steady_clock::now() + interval;
    timer.interval = interval;
    timer.callback = callback;
    timer.active = true;
    
    timers_[timer_id] = timer;
    
    Logger::get_instance().info("Registered timer with ID: " + std::to_string(timer_id));
    return timer_id;
}

void EventLoop::cancel_timer(TimerId timer_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = timers_.find(timer_id);
    if (it != timers_.end()) {
        it->second.active = false;
        Logger::get_instance().info("Cancelled timer with ID: " + std::to_string(timer_id));
    }
}

void EventLoop::post_event(Callback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    pending_events_.push_back(callback);
}

void EventLoop::run() {
    while (running_) {
        auto now = std::chrono::steady_clock::now();
        auto next_timer = process_timers();
        
        // Process pending events
        std::vector<Callback> events_to_process;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            events_to_process.swap(pending_events_);
        }
        
        for (const auto& event : events_to_process) {
            try {
                event();
            } catch (const std::exception& e) {
                Logger::get_instance().error("Event callback failed: " + std::string(e.what()));
            }
        }
        
        // Sleep until next timer or 100ms
        auto sleep_duration = std::chrono::milliseconds(100);
        if (next_timer != std::chrono::steady_clock::time_point::max()) {
            auto time_until_next = std::chrono::duration_cast<std::chrono::milliseconds>(next_timer - now);
            if (time_until_next < sleep_duration) {
                sleep_duration = time_until_next;
            }
        }
        
        if (sleep_duration > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(sleep_duration);
        }
    }
}

std::chrono::steady_clock::time_point EventLoop::process_timers() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point next_fire = std::chrono::steady_clock::time_point::max();
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    for (auto it = timers_.begin(); it != timers_.end();) {
        auto& [timer_id, timer] = *it;
        
        if (!timer.active) {
            it = timers_.erase(it);
            continue;
        }
        
        if (timer.next_fire <= now) {
            try {
                timer.callback();
            } catch (const std::exception& e) {
                Logger::get_instance().error("Timer callback failed: " + std::string(e.what()));
            }
            
            timer.next_fire = now + timer.interval;
        }
        
        next_fire = std::min(next_fire, timer.next_fire);
        ++it;
    }
    
    return next_fire;
}