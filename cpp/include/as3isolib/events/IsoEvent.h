#pragma once

#include "flash/events/Event.h"
#include <memory>
#include <string>

namespace as3isolib {
namespace events {

/**
 * IsoEvent class equivalent to ActionScript's as3isolib.events.IsoEvent
 * Represents event objects for various isometric display events
 */
class IsoEvent : public flash::events::Event {
public:
    // Event type constants
    static const std::string INVALIDATE;
    static const std::string RENDER;
    static const std::string RENDER_COMPLETE;
    static const std::string MOVE;
    static const std::string RESIZE;
    static const std::string CHILD_ADDED;
    static const std::string CHILD_REMOVED;
    
    IsoEvent(const std::string& type, bool bubbles = false, bool cancelable = false);
    virtual ~IsoEvent() = default;
    
    std::unique_ptr<flash::events::Event> clone() const override;
    
    // Property data
    std::string propName;
    void* oldValue = nullptr;
    void* newValue = nullptr;
    
    // Convenience setters for typed values
    template<typename T>
    void setOldValue(const T& value) {
        // In a real implementation, you'd want proper type-safe storage
        // This is a simplified version
        oldValue = const_cast<void*>(static_cast<const void*>(&value));
    }
    
    template<typename T>
    void setNewValue(const T& value) {
        newValue = const_cast<void*>(static_cast<const void*>(&value));
    }
    
    template<typename T>
    T* getOldValue() const {
        return static_cast<T*>(oldValue);
    }
    
    template<typename T>
    T* getNewValue() const {
        return static_cast<T*>(newValue);
    }
};

} // namespace events
} // namespace as3isolib