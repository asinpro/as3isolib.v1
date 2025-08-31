#include "flash/display/DisplayObject.h"
#include <algorithm>
#include <stdexcept>

namespace flash {
namespace display {

// DisplayObject implementation
DisplayObject::DisplayObject() {
}

DisplayObject::~DisplayObject() {
}

void DisplayObject::setX(double value) {
    if (x_ != value) {
        x_ = value;
        // TODO: Dispatch MOVE event
    }
}

void DisplayObject::setY(double value) {
    if (y_ != value) {
        y_ = value;
        // TODO: Dispatch MOVE event
    }
}

Stage* DisplayObject::getStage() const {
    // Walk up the parent chain to find the stage
    DisplayObject* current = const_cast<DisplayObject*>(this);
    while (current && current->parent_) {
        current = current->parent_;
    }
    // TODO: Return proper Stage object
    return nullptr;
}

flash::geom::Rectangle DisplayObject::getBounds(DisplayObject* targetCoordinateSpace) const {
    // Simplified implementation - return object bounds
    return flash::geom::Rectangle(x_, y_, width_, height_);
}

flash::geom::Rectangle DisplayObject::getRect(DisplayObject* targetCoordinateSpace) const {
    return getBounds(targetCoordinateSpace);
}

flash::geom::Point DisplayObject::globalToLocal(const flash::geom::Point& point) const {
    // Simplified implementation
    flash::geom::Matrix transform = getTransformToStage();
    transform.invert();
    return transform.transformPoint(point);
}

flash::geom::Point DisplayObject::localToGlobal(const flash::geom::Point& point) const {
    flash::geom::Matrix transform = getTransformToStage();
    return transform.transformPoint(point);
}

bool DisplayObject::hitTestPoint(double x, double y, bool shapeFlag) const {
    if (!visible_ || !mouseEnabled_) return false;
    
    // Simple rectangle hit test
    return x >= x_ && x <= x_ + width_ && y >= y_ && y <= y_ + height_;
}

bool DisplayObject::hitTestObject(DisplayObject* obj) const {
    if (!obj || !visible_ || !obj->visible_) return false;
    
    flash::geom::Rectangle bounds1 = getBounds(nullptr);
    flash::geom::Rectangle bounds2 = obj->getBounds(nullptr);
    return bounds1.intersects(bounds2);
}

void DisplayObject::render(const flash::geom::Matrix& transform) const {
    if (!visible_ || alpha_ <= 0.0) return;
    
    flash::geom::Matrix localTransform = transform;
    localTransform.translate(x_, y_);
    localTransform.scale(scaleX_, scaleY_);
    localTransform.rotate(rotation_ * PI / 180.0);
    
    renderImpl(localTransform);
}

flash::geom::Matrix DisplayObject::getTransformToStage() const {
    flash::geom::Matrix transform;
    const DisplayObject* current = this;
    
    while (current) {
        flash::geom::Matrix localTransform;
        localTransform.translate(current->x_, current->y_);
        localTransform.scale(current->scaleX_, current->scaleY_);
        localTransform.rotate(current->rotation_ * PI / 180.0);
        
        transform.concat(localTransform);
        current = current->parent_;
    }
    
    return transform;
}

// InteractiveObject implementation
InteractiveObject::InteractiveObject() {
}

// DisplayObjectContainer implementation
DisplayObjectContainer::DisplayObjectContainer() {
}

DisplayObjectContainer::~DisplayObjectContainer() {
    removeChildren();
}

DisplayObject* DisplayObjectContainer::addChild(std::shared_ptr<DisplayObject> child) {
    return addChildAt(child, getNumChildren());
}

DisplayObject* DisplayObjectContainer::addChildAt(std::shared_ptr<DisplayObject> child, int index) {
    if (!child) return nullptr;
    
    if (index < 0 || index > getNumChildren()) {
        throw std::out_of_range("Index out of range");
    }
    
    // Remove from current parent if any
    if (child->parent_) {
        child->parent_->removeChild(child.get());
    }
    
    // Insert at specified index
    children_.insert(children_.begin() + index, child);
    child->setParent(this);
    
    // TODO: Dispatch ADDED event
    
    return child.get();
}

std::shared_ptr<DisplayObject> DisplayObjectContainer::removeChild(DisplayObject* child) {
    if (!child) return nullptr;
    
    auto it = std::find_if(children_.begin(), children_.end(),
        [child](const std::shared_ptr<DisplayObject>& ptr) {
            return ptr.get() == child;
        });
    
    if (it != children_.end()) {
        std::shared_ptr<DisplayObject> removed = *it;
        children_.erase(it);
        removed->setParent(nullptr);
        
        // TODO: Dispatch REMOVED event
        
        return removed;
    }
    
    return nullptr;
}

std::shared_ptr<DisplayObject> DisplayObjectContainer::removeChildAt(int index) {
    validateIndex(index);
    
    std::shared_ptr<DisplayObject> child = children_[index];
    children_.erase(children_.begin() + index);
    child->setParent(nullptr);
    
    // TODO: Dispatch REMOVED event
    
    return child;
}

void DisplayObjectContainer::removeChildren(int beginIndex, int endIndex) {
    if (endIndex == -1) {
        endIndex = getNumChildren() - 1;
    }
    
    for (int i = endIndex; i >= beginIndex; --i) {
        removeChildAt(i);
    }
}

DisplayObject* DisplayObjectContainer::getChildAt(int index) const {
    validateIndex(index);
    return children_[index].get();
}

DisplayObject* DisplayObjectContainer::getChildByName(const std::string& name) const {
    for (const auto& child : children_) {
        if (child->getName() == name) {
            return child.get();
        }
    }
    return nullptr;
}

int DisplayObjectContainer::getChildIndex(DisplayObject* child) const {
    for (size_t i = 0; i < children_.size(); ++i) {
        if (children_[i].get() == child) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void DisplayObjectContainer::setChildIndex(DisplayObject* child, int index) {
    int currentIndex = getChildIndex(child);
    if (currentIndex == -1) {
        throw std::invalid_argument("Child not found");
    }
    
    if (index < 0 || index >= getNumChildren()) {
        throw std::out_of_range("Index out of range");
    }
    
    if (currentIndex != index) {
        std::shared_ptr<DisplayObject> childPtr = children_[currentIndex];
        children_.erase(children_.begin() + currentIndex);
        children_.insert(children_.begin() + index, childPtr);
    }
}

void DisplayObjectContainer::swapChildren(DisplayObject* child1, DisplayObject* child2) {
    int index1 = getChildIndex(child1);
    int index2 = getChildIndex(child2);
    
    if (index1 == -1 || index2 == -1) {
        throw std::invalid_argument("Child not found");
    }
    
    swapChildrenAt(index1, index2);
}

void DisplayObjectContainer::swapChildrenAt(int index1, int index2) {
    validateIndex(index1);
    validateIndex(index2);
    
    if (index1 != index2) {
        std::swap(children_[index1], children_[index2]);
    }
}

bool DisplayObjectContainer::contains(DisplayObject* child) const {
    return getChildIndex(child) != -1;
}

bool DisplayObjectContainer::hitTestPoint(double x, double y, bool shapeFlag) const {
    if (!visible_ || !mouseEnabled_) return false;
    
    if (mouseChildren_) {
        // Test children first (reverse order for proper z-ordering)
        for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
            if ((*it)->hitTestPoint(x, y, shapeFlag)) {
                return true;
            }
        }
    }
    
    return DisplayObject::hitTestPoint(x, y, shapeFlag);
}

void DisplayObjectContainer::render(const flash::geom::Matrix& transform) const {
    if (!visible_ || alpha_ <= 0.0) return;
    
    DisplayObject::render(transform);
}

void DisplayObjectContainer::renderImpl(const flash::geom::Matrix& transform) const {
    // Render all children
    for (const auto& child : children_) {
        child->render(transform);
    }
}

void DisplayObjectContainer::validateIndex(int index) const {
    if (index < 0 || index >= getNumChildren()) {
        throw std::out_of_range("Index out of range");
    }
}

// Sprite implementation
Sprite::Sprite() : graphics_(std::make_unique<Graphics>()) {
}

void Sprite::startDrag(bool lockCenter, const flash::geom::Rectangle* bounds) {
    isDragging_ = true;
    dragLockCenter_ = lockCenter;
    if (bounds) {
        dragBounds_ = *bounds;
    }
    // TODO: Implement drag logic with mouse input
}

void Sprite::stopDrag() {
    isDragging_ = false;
}

void Sprite::renderImpl(const flash::geom::Matrix& transform) const {
    // Render graphics first
    if (graphics_) {
        graphics_->render(transform);
    }
    
    // Then render children
    DisplayObjectContainer::renderImpl(transform);
}

// Shape implementation
Shape::Shape() : graphics_(std::make_unique<Graphics>()) {
}

void Shape::renderImpl(const flash::geom::Matrix& transform) const {
    if (graphics_) {
        graphics_->render(transform);
    }
}

// Graphics implementation
Graphics::Graphics() {
}

Graphics::~Graphics() {
}

void Graphics::clear() {
    commands_.clear();
    commands_.push_back({DrawCommand::CLEAR, {}, WHITE, 1.0});
    hasFill_ = false;
    hasStroke_ = false;
    currentX_ = currentY_ = 0.0;
}

void Graphics::beginFill(Color color, double alpha) {
    fillColor_ = color;
    fillAlpha_ = alpha;
    hasFill_ = true;
    commands_.push_back({DrawCommand::BEGIN_FILL, {}, color, alpha});
}

void Graphics::endFill() {
    hasFill_ = false;
    commands_.push_back({DrawCommand::END_FILL, {}, WHITE, 1.0});
}

void Graphics::lineStyle(double thickness, Color color, double alpha, bool pixelHinting) {
    strokeThickness_ = thickness;
    strokeColor_ = color;
    strokeAlpha_ = alpha;
    hasStroke_ = thickness > 0;
    commands_.push_back({DrawCommand::LINE_STYLE, {thickness}, color, alpha});
}

void Graphics::moveTo(double x, double y) {
    currentX_ = x;
    currentY_ = y;
    commands_.push_back({DrawCommand::MOVE_TO, {x, y}, WHITE, 1.0});
}

void Graphics::lineTo(double x, double y) {
    commands_.push_back({DrawCommand::LINE_TO, {currentX_, currentY_, x, y}, strokeColor_, strokeAlpha_});
    currentX_ = x;
    currentY_ = y;
}

void Graphics::curveTo(double controlX, double controlY, double anchorX, double anchorY) {
    commands_.push_back({DrawCommand::CURVE_TO, {currentX_, currentY_, controlX, controlY, anchorX, anchorY}, strokeColor_, strokeAlpha_});
    currentX_ = anchorX;
    currentY_ = anchorY;
}

void Graphics::drawRect(double x, double y, double width, double height) {
    commands_.push_back({DrawCommand::DRAW_RECT, {x, y, width, height}, fillColor_, fillAlpha_});
}

void Graphics::drawRoundRect(double x, double y, double width, double height, 
                           double ellipseWidth, double ellipseHeight) {
    if (ellipseHeight == 0.0) ellipseHeight = ellipseWidth;
    commands_.push_back({DrawCommand::DRAW_ROUND_RECT, {x, y, width, height, ellipseWidth, ellipseHeight}, fillColor_, fillAlpha_});
}

void Graphics::drawCircle(double x, double y, double radius) {
    commands_.push_back({DrawCommand::DRAW_CIRCLE, {x, y, radius}, fillColor_, fillAlpha_});
}

void Graphics::drawEllipse(double x, double y, double width, double height) {
    commands_.push_back({DrawCommand::DRAW_ELLIPSE, {x, y, width, height}, fillColor_, fillAlpha_});
}

void Graphics::render(const flash::geom::Matrix& transform) const {
    for (const auto& cmd : commands_) {
        Color renderColor = cmd.color;
        renderColor.a = static_cast<unsigned char>(cmd.alpha * 255);
        
        switch (cmd.type) {
            case DrawCommand::DRAW_RECT: {
                if (cmd.params.size() >= 4) {
                    flash::geom::Point topLeft = transform.transformPoint({cmd.params[0], cmd.params[1]});
                    flash::geom::Point bottomRight = transform.transformPoint({cmd.params[0] + cmd.params[2], cmd.params[1] + cmd.params[3]});
                    DrawRectangle(static_cast<int>(topLeft.x), static_cast<int>(topLeft.y), 
                                static_cast<int>(bottomRight.x - topLeft.x), 
                                static_cast<int>(bottomRight.y - topLeft.y), renderColor);
                }
                break;
            }
            case DrawCommand::DRAW_CIRCLE: {
                if (cmd.params.size() >= 3) {
                    flash::geom::Point center = transform.transformPoint({cmd.params[0], cmd.params[1]});
                    // Note: radius scaling would need more complex calculation for non-uniform transforms
                    double radius = cmd.params[2] * std::max(transform.a, transform.d);
                    DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), 
                             static_cast<float>(radius), renderColor);
                }
                break;
            }
            case DrawCommand::LINE_TO: {
                if (cmd.params.size() >= 4) {
                    flash::geom::Point start = transform.transformPoint({cmd.params[0], cmd.params[1]});
                    flash::geom::Point end = transform.transformPoint({cmd.params[2], cmd.params[3]});
                    DrawLine(static_cast<int>(start.x), static_cast<int>(start.y),
                           static_cast<int>(end.x), static_cast<int>(end.y), renderColor);
                }
                break;
            }
            // TODO: Implement other drawing commands
            default:
                break;
        }
    }
}

} // namespace display
} // namespace flash