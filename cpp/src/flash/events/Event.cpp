#include "flash/events/Event.h"

namespace flash {
namespace events {

Event::Event(const std::string& type, bool bubbles, bool cancelable)
    : type_(type), bubbles_(bubbles), cancelable_(cancelable) {
}

std::unique_ptr<Event> Event::clone() const {
    return std::make_unique<Event>(type_, bubbles_, cancelable_);
}

void EventDispatcher::addEventListener(const std::string& type, EventListener listener, 
                                     bool useCapture, int priority, bool useWeakReference) {
    ListenerInfo info{listener, useCapture, priority, useWeakReference};
    auto& listenerVec = listeners_[type];
    
    // Insert based on priority (higher priority first)
    auto it = listenerVec.begin();
    while (it != listenerVec.end() && it->priority >= priority) {
        ++it;
    }
    listenerVec.insert(it, info);
}

void EventDispatcher::removeEventListener(const std::string& type, EventListener listener, 
                                         bool useCapture) {
    auto it = listeners_.find(type);
    if (it != listeners_.end()) {
        auto& listenerVec = it->second;
        // Note: Function comparison is complex in C++, this is a simplified version
        // In practice, you might need to store function IDs or use other identification
        listenerVec.erase(
            std::remove_if(listenerVec.begin(), listenerVec.end(),
                [&](const ListenerInfo& info) {
                    return info.useCapture == useCapture;
                    // TODO: Add proper function comparison
                }),
            listenerVec.end()
        );
        
        if (listenerVec.empty()) {
            listeners_.erase(it);
        }
    }
}

bool EventDispatcher::dispatchEvent(std::unique_ptr<Event> event) {
    if (!event) return false;
    
    event->setCurrentTarget(this);
    if (!event->getTarget()) {
        event->setTarget(this);
    }
    
    auto it = listeners_.find(event->getType());
    if (it != listeners_.end()) {
        const auto& listenerVec = it->second;
        for (const auto& info : listenerVec) {
            if (event->isImmediatePropagationStopped()) {
                break;
            }
            info.listener(*event);
        }
    }
    
    return !event->isDefaultPrevented();
}

bool EventDispatcher::hasEventListener(const std::string& type) {
    auto it = listeners_.find(type);
    return it != listeners_.end() && !it->second.empty();
}

bool EventDispatcher::willTrigger(const std::string& type) {
    return hasEventListener(type);
}

} // namespace events
} // namespace flash