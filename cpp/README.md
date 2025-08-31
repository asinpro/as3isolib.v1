# as3isolib C++ Port

This is a C++ port of the ActionScript 3.0 as3isolib isometric library, using raylib for rendering.

## Overview

The original as3isolib is an ActionScript 3.0 library for creating isometric graphics and games in Flash. This C++ port provides equivalent functionality using modern C++ (C++17) and raylib for cross-platform rendering.

## Features

- **Flash API Equivalents**: Complete C++ implementations of Flash display objects (Sprite, DisplayObjectContainer, Shape, Graphics)
- **Event System**: ActionScript-style event dispatching and handling
- **3D Geometry**: Extended Point class (Pt) with z-coordinate support  
- **Isometric Math**: Utilities for converting between 3D isometric and 2D screen coordinates
- **Cross-Platform**: Built on raylib for Windows, macOS, Linux support
- **Unit Tested**: Comprehensive test suite using Catch2

## Building

### Prerequisites

- CMake 3.15+
- C++17 compatible compiler
- raylib dependencies (automatically downloaded if not found)

### Linux Dependencies

```bash
sudo apt install libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make -j4
```

### Build Options

- `BUILD_TESTS=ON/OFF` - Enable/disable unit tests (default: ON)
- `BUILD_EXAMPLES=ON/OFF` - Enable/disable examples (default: ON)

## Architecture

The library is organized into several key namespaces:

### flash::events
- `Event` - Base event class
- `EventDispatcher` - Event dispatching and listening
- `IEventDispatcher` - Event dispatcher interface

### flash::display
- `DisplayObject` - Base display object with position, scale, rotation
- `DisplayObjectContainer` - Container for child display objects
- `Sprite` - Interactive display object with graphics
- `Shape` - Simple display object with graphics
- `Graphics` - Vector drawing operations using raylib

### flash::geom
- `Point` - 2D point class
- `Rectangle` - Rectangle geometry
- `Matrix` - 2D transformation matrix

### as3isolib::geom
- `Pt` - 3D point class extending flash::geom::Point
- Static utility methods for 3D calculations

### as3isolib::events
- `IsoEvent` - Isometric-specific event types

## Usage Examples

### Basic 3D Point Operations

```cpp
#include "as3isolib/geom/Pt.h"

using namespace as3isolib::geom;

// Create 3D points
Pt point1(0, 0, 0);
Pt point2(10, 20, 30);

// Calculate distance
double distance = Pt::distance(point1, point2);

// Interpolate between points
Pt midpoint = Pt::interpolate(point1, point2, 0.5);

// Polar coordinates
Pt polar = Pt::polar(point1, 50.0, M_PI/4);
```

### Display Objects and Graphics

```cpp
#include "flash/display/DisplayObject.h"

using namespace flash::display;

// Create a sprite
auto sprite = std::make_unique<Sprite>();
sprite->setX(100);
sprite->setY(100);

// Draw graphics
Graphics* graphics = sprite->getGraphics();
graphics->beginFill(RED, 1.0);
graphics->drawRect(0, 0, 50, 50);
graphics->endFill();

// Add to container
auto container = std::make_unique<DisplayObjectContainer>();
container->addChild(std::move(sprite));
```

### Event Handling

```cpp
#include "flash/events/Event.h"
#include "as3isolib/events/IsoEvent.h"

using namespace flash::events;
using namespace as3isolib::events;

EventDispatcher dispatcher;

// Add event listener
dispatcher.addEventListener("customEvent", [](const Event& evt) {
    std::cout << "Event received: " << evt.getType() << std::endl;
});

// Dispatch event
auto event = std::make_unique<IsoEvent>(IsoEvent::RENDER);
dispatcher.dispatchEvent(std::move(event));
```

## Examples

The library includes two example programs:

### basic_example
Demonstrates basic functionality including 3D points, events, and simple graphics.

```bash
./examples/basic_example
```

### isometric_demo  
Interactive isometric grid renderer with camera controls.

```bash
./examples/isometric_demo
```

Use arrow keys to move the camera around the isometric scene.

## Running Tests

```bash
./tests/as3isolib_tests
```

Tests cover:
- 3D point mathematics and operations
- Event system functionality  
- Display object hierarchy and properties
- Graphics rendering setup

## Key Differences from ActionScript

1. **Memory Management**: Uses smart pointers (std::unique_ptr, std::shared_ptr) instead of garbage collection
2. **Type Safety**: Strong typing compared to ActionScript's dynamic typing
3. **Namespace Organization**: Uses C++ namespaces to replicate ActionScript packages
4. **Function Objects**: Uses std::function for event listeners instead of Function type
5. **Rendering**: Direct raylib calls instead of Flash's display list rendering

## API Compatibility

The C++ port maintains API compatibility with the original ActionScript library where possible:

- Method names and signatures closely match the original
- Event types and constants use the same names
- Mathematical operations produce identical results
- Display object hierarchy follows the same patterns

## Performance Considerations

- Display objects use efficient C++ containers (std::vector) for child lists
- Graphics operations are batched through raylib's optimized rendering
- 3D calculations leverage C++ performance vs ActionScript VM overhead
- Memory allocation is explicit and controllable

## Contributing

The library follows modern C++ best practices:
- RAII for resource management
- Smart pointers for memory safety
- Const-correctness where applicable
- Exception safety guarantees
- Comprehensive unit test coverage

## License

This port maintains the same MIT license as the original as3isolib project.