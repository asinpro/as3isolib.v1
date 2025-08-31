#include <catch2/catch_test_macros.hpp>
#include "flash/display/DisplayObject.h"
#include "flash/geom/Point.h"

using namespace flash::display;
using namespace flash::geom;

TEST_CASE("DisplayObject basic functionality", "[DisplayObject]") {
    SECTION("Position properties") {
        auto obj = std::make_unique<DisplayObject>();
        
        REQUIRE(obj->getX() == 0.0);
        REQUIRE(obj->getY() == 0.0);
        REQUIRE(obj->getZ() == 0.0);
        
        obj->setX(10.0);
        obj->setY(20.0);
        obj->setZ(30.0);
        
        REQUIRE(obj->getX() == 10.0);
        REQUIRE(obj->getY() == 20.0);
        REQUIRE(obj->getZ() == 30.0);
    }
    
    SECTION("Size properties") {
        auto obj = std::make_unique<DisplayObject>();
        
        REQUIRE(obj->getWidth() == 0.0);
        REQUIRE(obj->getHeight() == 0.0);
        
        obj->setWidth(100.0);
        obj->setHeight(200.0);
        
        REQUIRE(obj->getWidth() == 100.0);
        REQUIRE(obj->getHeight() == 200.0);
    }
    
    SECTION("Scale properties") {
        auto obj = std::make_unique<DisplayObject>();
        
        REQUIRE(obj->getScaleX() == 1.0);
        REQUIRE(obj->getScaleY() == 1.0);
        
        obj->setScaleX(2.0);
        obj->setScaleY(0.5);
        
        REQUIRE(obj->getScaleX() == 2.0);
        REQUIRE(obj->getScaleY() == 0.5);
    }
    
    SECTION("Visibility and alpha") {
        auto obj = std::make_unique<DisplayObject>();
        
        REQUIRE(obj->getVisible() == true);
        REQUIRE(obj->getAlpha() == 1.0);
        
        obj->setVisible(false);
        obj->setAlpha(0.5);
        
        REQUIRE(obj->getVisible() == false);
        REQUIRE(obj->getAlpha() == 0.5);
    }
    
    SECTION("Alpha clamping") {
        auto obj = std::make_unique<DisplayObject>();
        
        obj->setAlpha(-0.5); // Should be clamped to 0
        REQUIRE(obj->getAlpha() == 0.0);
        
        obj->setAlpha(1.5); // Should be clamped to 1
        REQUIRE(obj->getAlpha() == 1.0);
    }
    
    SECTION("Name property") {
        auto obj = std::make_unique<DisplayObject>();
        
        REQUIRE(obj->getName().empty());
        
        obj->setName("testObject");
        REQUIRE(obj->getName() == "testObject");
    }
}

TEST_CASE("DisplayObjectContainer functionality", "[DisplayObjectContainer]") {
    SECTION("Child management") {
        auto container = std::make_unique<DisplayObjectContainer>();
        auto child1 = std::make_shared<DisplayObject>();
        auto child2 = std::make_shared<DisplayObject>();
        
        child1->setName("child1");
        child2->setName("child2");
        
        REQUIRE(container->getNumChildren() == 0);
        
        container->addChild(child1);
        REQUIRE(container->getNumChildren() == 1);
        REQUIRE(container->getChildAt(0) == child1.get());
        REQUIRE(child1->getParent() == container.get());
        
        container->addChild(child2);
        REQUIRE(container->getNumChildren() == 2);
        REQUIRE(container->getChildAt(1) == child2.get());
    }
    
    SECTION("Child removal") {
        auto container = std::make_unique<DisplayObjectContainer>();
        auto child1 = std::make_shared<DisplayObject>();
        auto child2 = std::make_shared<DisplayObject>();
        
        container->addChild(child1);
        container->addChild(child2);
        REQUIRE(container->getNumChildren() == 2);
        
        auto removed = container->removeChild(child1.get());
        REQUIRE(removed == child1);
        REQUIRE(container->getNumChildren() == 1);
        REQUIRE(child1->getParent() == nullptr);
        
        container->removeChildAt(0);
        REQUIRE(container->getNumChildren() == 0);
    }
    
    SECTION("Child lookup") {
        auto container = std::make_unique<DisplayObjectContainer>();
        auto child = std::make_shared<DisplayObject>();
        child->setName("testChild");
        
        container->addChild(child);
        
        REQUIRE(container->getChildByName("testChild") == child.get());
        REQUIRE(container->getChildByName("nonexistent") == nullptr);
        REQUIRE(container->getChildIndex(child.get()) == 0);
        REQUIRE(container->contains(child.get()) == true);
    }
    
    SECTION("Child reordering") {
        auto container = std::make_unique<DisplayObjectContainer>();
        auto child1 = std::make_shared<DisplayObject>();
        auto child2 = std::make_shared<DisplayObject>();
        auto child3 = std::make_shared<DisplayObject>();
        
        child1->setName("first");
        child2->setName("second");
        child3->setName("third");
        
        container->addChild(child1);
        container->addChild(child2);
        container->addChild(child3);
        
        // Test swapping
        container->swapChildren(child1.get(), child3.get());
        REQUIRE(container->getChildAt(0) == child3.get());
        REQUIRE(container->getChildAt(2) == child1.get());
        
        // Test index setting
        container->setChildIndex(child2.get(), 0);
        REQUIRE(container->getChildAt(0) == child2.get());
    }
}

TEST_CASE("Sprite functionality", "[Sprite]") {
    SECTION("Graphics access") {
        auto sprite = std::make_unique<Sprite>();
        
        REQUIRE(sprite->getGraphics() != nullptr);
    }
    
    SECTION("Button properties") {
        auto sprite = std::make_unique<Sprite>();
        
        REQUIRE(sprite->getButtonMode() == false);
        REQUIRE(sprite->getUseHandCursor() == true);
        
        sprite->setButtonMode(true);
        sprite->setUseHandCursor(false);
        
        REQUIRE(sprite->getButtonMode() == true);
        REQUIRE(sprite->getUseHandCursor() == false);
    }
    
    SECTION("Drag functionality") {
        auto sprite = std::make_unique<Sprite>();
        
        // Test start/stop drag (basic functionality test)
        sprite->startDrag();
        sprite->stopDrag();
        
        flash::geom::Rectangle bounds(0, 0, 100, 100);
        sprite->startDrag(true, &bounds);
        sprite->stopDrag();
        
        // If we get here without crashes, basic functionality works
        REQUIRE(true);
    }
}

TEST_CASE("Graphics functionality", "[Graphics]") {
    SECTION("Basic drawing commands") {
        auto shape = std::make_unique<Shape>();
        Graphics* graphics = shape->getGraphics();
        
        REQUIRE(graphics != nullptr);
        
        // Test basic drawing operations don't crash
        graphics->clear();
        graphics->beginFill(RED, 0.5);
        graphics->drawRect(10, 10, 50, 30);
        graphics->endFill();
        
        graphics->lineStyle(2.0, BLUE);
        graphics->moveTo(0, 0);
        graphics->lineTo(100, 100);
        
        graphics->drawCircle(50, 50, 25);
        
        // If we get here without crashes, basic functionality works
        REQUIRE(true);
    }
}