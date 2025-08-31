#pragma once

#include "flash/events/Event.h"
#include "flash/geom/Point.h"
#include <raylib.h>
#include <vector>
#include <memory>
#include <string>

namespace flash {
namespace display {

// Forward declarations
class DisplayObjectContainer;
class Stage;
class Graphics;

/**
 * DisplayObject class equivalent to ActionScript's flash.display.DisplayObject
 * Base class for all display objects
 */
class DisplayObject : public flash::events::EventDispatcher {
    friend class DisplayObjectContainer; // Allow container to access protected members
public:
    DisplayObject();
    virtual ~DisplayObject();
    
    // Position properties
    double getX() const { return x_; }
    void setX(double value);
    
    double getY() const { return y_; }
    void setY(double value);
    
    double getZ() const { return z_; }
    void setZ(double value) { z_ = value; }
    
    // Size properties
    double getWidth() const { return width_; }
    virtual void setWidth(double value) { width_ = value; }
    
    double getHeight() const { return height_; }
    virtual void setHeight(double value) { height_ = value; }
    
    // Scale properties
    double getScaleX() const { return scaleX_; }
    void setScaleX(double value) { scaleX_ = value; }
    
    double getScaleY() const { return scaleY_; }
    void setScaleY(double value) { scaleY_ = value; }
    
    // Rotation
    double getRotation() const { return rotation_; }
    void setRotation(double value) { rotation_ = value; }
    
    // Visibility
    bool getVisible() const { return visible_; }
    void setVisible(bool value) { visible_ = value; }
    
    // Alpha
    double getAlpha() const { return alpha_; }
    void setAlpha(double value) { alpha_ = std::max(0.0, std::min(1.0, value)); }
    
    // Mouse properties
    bool getMouseEnabled() const { return mouseEnabled_; }
    void setMouseEnabled(bool value) { mouseEnabled_ = value; }
    
    // Parent-child relationship
    DisplayObjectContainer* getParent() const { return parent_; }
    void setParent(DisplayObjectContainer* value) { parent_ = value; }
    
    Stage* getStage() const;
    
    // Bounds
    flash::geom::Rectangle getBounds(DisplayObject* targetCoordinateSpace) const;
    flash::geom::Rectangle getRect(DisplayObject* targetCoordinateSpace) const;
    
    // Coordinate transformations
    flash::geom::Point globalToLocal(const flash::geom::Point& point) const;
    flash::geom::Point localToGlobal(const flash::geom::Point& point) const;
    
    // Hit testing
    virtual bool hitTestPoint(double x, double y, bool shapeFlag = false) const;
    virtual bool hitTestObject(DisplayObject* obj) const;
    
    // Name
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    // Render method (to be called by the rendering system)
    virtual void render(const flash::geom::Matrix& transform = flash::geom::Matrix()) const;

protected:
    double x_ = 0.0;
    double y_ = 0.0;
    double z_ = 0.0;
    double width_ = 0.0;
    double height_ = 0.0;
    double scaleX_ = 1.0;
    double scaleY_ = 1.0;
    double rotation_ = 0.0;
    double alpha_ = 1.0;
    bool visible_ = true;
    bool mouseEnabled_ = true;
    std::string name_;
    
    DisplayObjectContainer* parent_ = nullptr;
    
    // Get the accumulated transform from this object to the stage
    flash::geom::Matrix getTransformToStage() const;
    
    // Virtual render implementation
    virtual void renderImpl(const flash::geom::Matrix& transform) const {}
};

/**
 * InteractiveObject class equivalent to ActionScript's flash.display.InteractiveObject
 */
class InteractiveObject : public DisplayObject {
public:
    InteractiveObject();
    virtual ~InteractiveObject() = default;
    
    bool getDoubleClickEnabled() const { return doubleClickEnabled_; }
    void setDoubleClickEnabled(bool value) { doubleClickEnabled_ = value; }
    
    bool getFocusRect() const { return focusRect_; }
    void setFocusRect(bool value) { focusRect_ = value; }
    
    bool getTabEnabled() const { return tabEnabled_; }
    void setTabEnabled(bool value) { tabEnabled_ = value; }
    
    int getTabIndex() const { return tabIndex_; }
    void setTabIndex(int value) { tabIndex_ = value; }

protected:
    bool doubleClickEnabled_ = false;
    bool focusRect_ = true;
    bool tabEnabled_ = false;
    int tabIndex_ = -1;
};

/**
 * DisplayObjectContainer class equivalent to ActionScript's flash.display.DisplayObjectContainer
 */
class DisplayObjectContainer : public InteractiveObject {
public:
    DisplayObjectContainer();
    virtual ~DisplayObjectContainer();
    
    // Child management
    DisplayObject* addChild(std::shared_ptr<DisplayObject> child);
    DisplayObject* addChildAt(std::shared_ptr<DisplayObject> child, int index);
    std::shared_ptr<DisplayObject> removeChild(DisplayObject* child);
    std::shared_ptr<DisplayObject> removeChildAt(int index);
    void removeChildren(int beginIndex = 0, int endIndex = -1);
    
    DisplayObject* getChildAt(int index) const;
    DisplayObject* getChildByName(const std::string& name) const;
    int getChildIndex(DisplayObject* child) const;
    void setChildIndex(DisplayObject* child, int index);
    void swapChildren(DisplayObject* child1, DisplayObject* child2);
    void swapChildrenAt(int index1, int index2);
    
    bool contains(DisplayObject* child) const;
    
    int getNumChildren() const { return static_cast<int>(children_.size()); }
    
    // Mouse properties
    bool getMouseChildren() const { return mouseChildren_; }
    void setMouseChildren(bool value) { mouseChildren_ = value; }
    
    bool getTabChildren() const { return tabChildren_; }
    void setTabChildren(bool value) { tabChildren_ = value; }
    
    // Hit testing
    bool hitTestPoint(double x, double y, bool shapeFlag = false) const override;
    
    // Rendering
    void render(const flash::geom::Matrix& transform = flash::geom::Matrix()) const override;

protected:
    std::vector<std::shared_ptr<DisplayObject>> children_;
    bool mouseChildren_ = true;
    bool tabChildren_ = true;
    
    void renderImpl(const flash::geom::Matrix& transform) const override;
    
private:
    void validateIndex(int index) const;
};

/**
 * Sprite class equivalent to ActionScript's flash.display.Sprite
 */
class Sprite : public DisplayObjectContainer {
public:
    Sprite();
    virtual ~Sprite() = default;
    
    Graphics* getGraphics() const { return graphics_.get(); }
    
    bool getButtonMode() const { return buttonMode_; }
    void setButtonMode(bool value) { buttonMode_ = value; }
    
    bool getDropTarget() const { return dropTarget_; }
    
    bool getUseHandCursor() const { return useHandCursor_; }
    void setUseHandCursor(bool value) { useHandCursor_ = value; }
    
    void startDrag(bool lockCenter = false, 
                   const flash::geom::Rectangle* bounds = nullptr);
    void stopDrag();

protected:
    std::unique_ptr<Graphics> graphics_;
    bool buttonMode_ = false;
    bool dropTarget_ = false;
    bool useHandCursor_ = true;
    bool isDragging_ = false;
    bool dragLockCenter_ = false;
    flash::geom::Rectangle dragBounds_;
    flash::geom::Point dragOffset_;
    
    void renderImpl(const flash::geom::Matrix& transform) const override;
};

/**
 * Shape class equivalent to ActionScript's flash.display.Shape
 */
class Shape : public DisplayObject {
public:
    Shape();
    virtual ~Shape() = default;
    
    Graphics* getGraphics() const { return graphics_.get(); }

protected:
    std::unique_ptr<Graphics> graphics_;
    
    void renderImpl(const flash::geom::Matrix& transform) const override;
};

/**
 * Graphics class equivalent to ActionScript's flash.display.Graphics
 * Provides vector drawing capabilities using raylib
 */
class Graphics {
public:
    Graphics();
    ~Graphics();
    
    // Drawing methods
    void clear();
    void beginFill(Color color, double alpha = 1.0);
    void endFill();
    void lineStyle(double thickness = 1.0, Color color = BLACK, 
                   double alpha = 1.0, bool pixelHinting = false);
    
    // Drawing primitives
    void moveTo(double x, double y);
    void lineTo(double x, double y);
    void curveTo(double controlX, double controlY, double anchorX, double anchorY);
    void drawRect(double x, double y, double width, double height);
    void drawRoundRect(double x, double y, double width, double height, 
                       double ellipseWidth, double ellipseHeight = 0.0);
    void drawCircle(double x, double y, double radius);
    void drawEllipse(double x, double y, double width, double height);
    
    // Render the graphics using raylib
    void render(const flash::geom::Matrix& transform) const;

private:
    struct DrawCommand {
        enum Type {
            MOVE_TO,
            LINE_TO,
            CURVE_TO,
            DRAW_RECT,
            DRAW_ROUND_RECT,
            DRAW_CIRCLE,
            DRAW_ELLIPSE,
            BEGIN_FILL,
            END_FILL,
            LINE_STYLE,
            CLEAR
        };
        
        Type type;
        std::vector<double> params;
        Color color;
        double alpha;
    };
    
    std::vector<DrawCommand> commands_;
    bool hasFill_ = false;
    Color fillColor_ = WHITE;
    double fillAlpha_ = 1.0;
    bool hasStroke_ = false;
    Color strokeColor_ = BLACK;
    double strokeThickness_ = 1.0;
    double strokeAlpha_ = 1.0;
    double currentX_ = 0.0;
    double currentY_ = 0.0;
};

} // namespace display
} // namespace flash