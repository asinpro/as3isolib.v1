#include <raylib.h>
#include "flash/display/DisplayObject.h"
#include "flash/events/Event.h"
#include "as3isolib/geom/Pt.h"
#include "as3isolib/events/IsoEvent.h"
#include <iostream>
#include <memory>

using namespace flash::display;
using namespace flash::events;
using namespace as3isolib::geom;
using namespace as3isolib::events;

int main() {
    // Initialize raylib
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "as3isolib C++ - Basic Example");
    SetTargetFPS(60);
    
    // Create some display objects
    auto sprite = std::make_unique<Sprite>();
    sprite->setX(100);
    sprite->setY(100);
    sprite->setWidth(50);
    sprite->setHeight(50);
    sprite->setName("exampleSprite");
    
    // Create a 3D point
    Pt point3D(10.0, 20.0, 30.0);
    std::cout << "3D Point: " << point3D.toString() << std::endl;
    std::cout << "3D Point Length: " << point3D.getLength() << std::endl;
    
    // Test point operations
    Pt point1(0, 0, 0);
    Pt point2(10, 10, 10);
    double distance = Pt::distance(point1, point2);
    std::cout << "Distance between points: " << distance << std::endl;
    
    Pt interpolated = Pt::interpolate(point1, point2, 0.5);
    std::cout << "Interpolated point: " << interpolated.toString() << std::endl;
    
    // Test events
    EventDispatcher dispatcher;
    bool eventReceived = false;
    
    dispatcher.addEventListener("testEvent", [&eventReceived](const Event& evt) {
        std::cout << "Event received: " << evt.getType() << std::endl;
        eventReceived = true;
    });
    
    auto testEvent = std::make_unique<Event>("testEvent", false, false);
    dispatcher.dispatchEvent(std::move(testEvent));
    
    // Test IsoEvent
    IsoEvent isoEvent(IsoEvent::RENDER, true, false);
    isoEvent.propName = "exampleProperty";
    std::cout << "IsoEvent type: " << isoEvent.getType() << std::endl;
    
    // Draw some basic graphics
    auto shape = std::make_unique<Shape>();
    Graphics* graphics = shape->getGraphics();
    graphics->beginFill(RED, 1.0);
    graphics->drawRect(0, 0, 100, 50);
    graphics->endFill();
    
    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw some basic shapes to demonstrate raylib integration
        DrawRectangle(static_cast<int>(sprite->getX()), static_cast<int>(sprite->getY()), 
                     static_cast<int>(sprite->getWidth()), static_cast<int>(sprite->getHeight()), 
                     BLUE);
        
        DrawText("as3isolib C++ Basic Example", 10, 10, 20, DARKGRAY);
        DrawText("Press ESC to exit", 10, 40, 10, GRAY);
        
        // Display some information
        std::string info = "Sprite: " + sprite->getName() + 
                          " at (" + std::to_string(sprite->getX()) + 
                          ", " + std::to_string(sprite->getY()) + ")";
        DrawText(info.c_str(), 10, screenHeight - 60, 10, DARKGRAY);
        
        std::string pointInfo = "3D Point: " + point3D.toString();
        DrawText(pointInfo.c_str(), 10, screenHeight - 40, 10, DARKGRAY);
        
        std::string eventInfo = "Event received: " + std::string(eventReceived ? "Yes" : "No");
        DrawText(eventInfo.c_str(), 10, screenHeight - 20, 10, DARKGRAY);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}