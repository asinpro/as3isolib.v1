#include "as3isolib/events/IsoEvent.h"

namespace as3isolib {
namespace events {

// Define event type constants
const std::string IsoEvent::INVALIDATE = "as3isolib_invalidate";
const std::string IsoEvent::RENDER = "as3isolib_render";
const std::string IsoEvent::RENDER_COMPLETE = "as3isolib_renderComplete";
const std::string IsoEvent::MOVE = "as3isolib_move";
const std::string IsoEvent::RESIZE = "as3isolib_resize";
const std::string IsoEvent::CHILD_ADDED = "as3isolib_childAdded";
const std::string IsoEvent::CHILD_REMOVED = "as3isolib_childRemoved";

IsoEvent::IsoEvent(const std::string& type, bool bubbles, bool cancelable)
    : flash::events::Event(type, bubbles, cancelable) {
}

std::unique_ptr<flash::events::Event> IsoEvent::clone() const {
    auto evt = std::make_unique<IsoEvent>(getType(), getBubbles(), getCancelable());
    evt->propName = propName;
    evt->oldValue = oldValue;
    evt->newValue = newValue;
    return evt;
}

} // namespace events
} // namespace as3isolib