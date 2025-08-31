#include <catch2/catch_test_macros.hpp>
#include "as3isolib/events/IsoEvent.h"
#include "flash/events/Event.h"

using namespace as3isolib::events;
using namespace flash::events;

TEST_CASE("IsoEvent basic functionality", "[IsoEvent]") {
    SECTION("Constructor and basic properties") {
        IsoEvent evt(IsoEvent::RENDER, true, false);
        REQUIRE(evt.getType() == IsoEvent::RENDER);
        REQUIRE(evt.getBubbles() == true);
        REQUIRE(evt.getCancelable() == false);
    }
    
    SECTION("Event constants") {
        REQUIRE(IsoEvent::INVALIDATE == "as3isolib_invalidate");
        REQUIRE(IsoEvent::RENDER == "as3isolib_render");
        REQUIRE(IsoEvent::RENDER_COMPLETE == "as3isolib_renderComplete");
        REQUIRE(IsoEvent::MOVE == "as3isolib_move");
        REQUIRE(IsoEvent::RESIZE == "as3isolib_resize");
        REQUIRE(IsoEvent::CHILD_ADDED == "as3isolib_childAdded");
        REQUIRE(IsoEvent::CHILD_REMOVED == "as3isolib_childRemoved");
    }
    
    SECTION("Property data") {
        IsoEvent evt(IsoEvent::MOVE);
        evt.propName = "currentPt";
        
        double oldVal = 10.0;
        double newVal = 20.0;
        evt.setOldValue(oldVal);
        evt.setNewValue(newVal);
        
        REQUIRE(evt.propName == "currentPt");
        REQUIRE(*evt.getOldValue<double>() == 10.0);
        REQUIRE(*evt.getNewValue<double>() == 20.0);
    }
    
    SECTION("Clone functionality") {
        IsoEvent original(IsoEvent::RESIZE, true, true);
        original.propName = "test";
        
        auto cloned = original.clone();
        auto isoCloned = dynamic_cast<IsoEvent*>(cloned.get());
        
        REQUIRE(isoCloned != nullptr);
        REQUIRE(isoCloned->getType() == original.getType());
        REQUIRE(isoCloned->getBubbles() == original.getBubbles());
        REQUIRE(isoCloned->getCancelable() == original.getCancelable());
        REQUIRE(isoCloned->propName == original.propName);
    }
}

TEST_CASE("Flash Event basic functionality", "[Event]") {
    SECTION("Constructor and properties") {
        Event evt("testEvent", true, false);
        REQUIRE(evt.getType() == "testEvent");
        REQUIRE(evt.getBubbles() == true);
        REQUIRE(evt.getCancelable() == false);
        REQUIRE(evt.isDefaultPrevented() == false);
        REQUIRE(evt.isPropagationStopped() == false);
    }
    
    SECTION("Event control methods") {
        Event evt("testEvent", true, true);
        
        evt.preventDefault();
        REQUIRE(evt.isDefaultPrevented() == true);
        
        evt.stopPropagation();
        REQUIRE(evt.isPropagationStopped() == true);
        
        evt.stopImmediatePropagation();
        REQUIRE(evt.isImmediatePropagationStopped() == true);
    }
    
    SECTION("Clone functionality") {
        Event original("testEvent", true, false);
        auto cloned = original.clone();
        
        REQUIRE(cloned->getType() == original.getType());
        REQUIRE(cloned->getBubbles() == original.getBubbles());
        REQUIRE(cloned->getCancelable() == original.getCancelable());
    }
}

TEST_CASE("EventDispatcher functionality", "[EventDispatcher]") {
    SECTION("Add and check listeners") {
        EventDispatcher dispatcher;
        bool listenerCalled = false;
        
        auto listener = [&listenerCalled](const Event& evt) {
            listenerCalled = true;
        };
        
        REQUIRE(dispatcher.hasEventListener("testEvent") == false);
        
        dispatcher.addEventListener("testEvent", listener);
        REQUIRE(dispatcher.hasEventListener("testEvent") == true);
        REQUIRE(dispatcher.willTrigger("testEvent") == true);
    }
    
    SECTION("Dispatch events") {
        EventDispatcher dispatcher;
        std::string receivedType;
        
        auto listener = [&receivedType](const Event& evt) {
            receivedType = evt.getType();
        };
        
        dispatcher.addEventListener("testEvent", listener);
        
        auto event = std::make_unique<Event>("testEvent");
        bool dispatched = dispatcher.dispatchEvent(std::move(event));
        
        REQUIRE(dispatched == true);
        REQUIRE(receivedType == "testEvent");
    }
    
    SECTION("Event cancellation") {
        EventDispatcher dispatcher;
        
        auto listener = [](const Event& evt) {
            // This would need a mutable reference to modify the event
            // Simplified for this test
        };
        
        dispatcher.addEventListener("testEvent", listener);
        
        auto event = std::make_unique<Event>("testEvent", false, true);
        event->preventDefault(); // Cancel the event
        bool dispatched = dispatcher.dispatchEvent(std::move(event));
        
        REQUIRE(dispatched == false); // Should be false when cancelled
    }
}