#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>

namespace flash {
namespace events {

// Forward declarations
class Event;
class IEventDispatcher;

/**
 * Event class equivalent to ActionScript's flash.events.Event
 */
class Event {
public:
    Event(const std::string& type, bool bubbles = false, bool cancelable = false);
    virtual ~Event() = default;
    
    virtual std::unique_ptr<Event> clone() const;
    
    const std::string& getType() const { return type_; }
    bool getBubbles() const { return bubbles_; }
    bool getCancelable() const { return cancelable_; }
    
    void preventDefault() { defaultPrevented_ = true; }
    bool isDefaultPrevented() const { return defaultPrevented_; }
    
    void stopPropagation() { propagationStopped_ = true; }
    bool isPropagationStopped() const { return propagationStopped_; }
    
    void stopImmediatePropagation() { immediatePropagationStopped_ = true; }
    bool isImmediatePropagationStopped() const { return immediatePropagationStopped_; }
    
    IEventDispatcher* getCurrentTarget() const { return currentTarget_; }
    void setCurrentTarget(IEventDispatcher* target) { currentTarget_ = target; }
    
    IEventDispatcher* getTarget() const { return target_; }
    void setTarget(IEventDispatcher* target) { target_ = target; }

protected:
    std::string type_;
    bool bubbles_;
    bool cancelable_;
    bool defaultPrevented_ = false;
    bool propagationStopped_ = false;
    bool immediatePropagationStopped_ = false;
    IEventDispatcher* currentTarget_ = nullptr;
    IEventDispatcher* target_ = nullptr;
};

using EventListener = std::function<void(const Event&)>;

/**
 * IEventDispatcher interface equivalent to ActionScript's flash.events.IEventDispatcher
 */
class IEventDispatcher {
public:
    virtual ~IEventDispatcher() = default;
    
    virtual void addEventListener(const std::string& type, EventListener listener, 
                                bool useCapture = false, int priority = 0, 
                                bool useWeakReference = false) = 0;
    virtual void removeEventListener(const std::string& type, EventListener listener, 
                                   bool useCapture = false) = 0;
    virtual bool dispatchEvent(std::unique_ptr<Event> event) = 0;
    virtual bool hasEventListener(const std::string& type) = 0;
    virtual bool willTrigger(const std::string& type) = 0;
};

/**
 * EventDispatcher class equivalent to ActionScript's flash.events.EventDispatcher
 */
class EventDispatcher : public IEventDispatcher {
public:
    EventDispatcher() = default;
    virtual ~EventDispatcher() = default;
    
    void addEventListener(const std::string& type, EventListener listener, 
                         bool useCapture = false, int priority = 0, 
                         bool useWeakReference = false) override;
    void removeEventListener(const std::string& type, EventListener listener, 
                           bool useCapture = false) override;
    bool dispatchEvent(std::unique_ptr<Event> event) override;
    bool hasEventListener(const std::string& type) override;
    bool willTrigger(const std::string& type) override;

protected:
    struct ListenerInfo {
        EventListener listener;
        bool useCapture;
        int priority;
        bool useWeakReference;
    };
    
    std::unordered_map<std::string, std::vector<ListenerInfo>> listeners_;
};

} // namespace events
} // namespace flash