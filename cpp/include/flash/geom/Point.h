#pragma once

#include <cmath>
#include <string>

namespace flash {
namespace geom {

/**
 * Point class equivalent to ActionScript's flash.geom.Point
 */
class Point {
public:
    double x = 0.0;
    double y = 0.0;
    
    Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}
    virtual ~Point() = default;
    
    virtual Point clone() const { return Point(x, y); }
    
    double getLength() const { 
        return std::sqrt(x * x + y * y); 
    }
    
    void normalize(double thickness = 1.0) {
        double len = getLength();
        if (len > 0) {
            x = (x / len) * thickness;
            y = (y / len) * thickness;
        }
    }
    
    void offset(double dx, double dy) {
        x += dx;
        y += dy;
    }
    
    bool equals(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    Point subtract(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    
    Point add(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    
    std::string toString() const {
        return "(x=" + std::to_string(x) + ", y=" + std::to_string(y) + ")";
    }
    
    static double distance(const Point& pt1, const Point& pt2) {
        double dx = pt2.x - pt1.x;
        double dy = pt2.y - pt1.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    static Point interpolate(const Point& pt1, const Point& pt2, double f) {
        return Point(
            pt1.x + (pt2.x - pt1.x) * f,
            pt1.y + (pt2.y - pt1.y) * f
        );
    }
    
    static Point polar(double len, double angle) {
        return Point(len * std::cos(angle), len * std::sin(angle));
    }
};

/**
 * Rectangle class equivalent to ActionScript's flash.geom.Rectangle
 */
class Rectangle {
public:
    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;
    
    Rectangle(double x = 0.0, double y = 0.0, double width = 0.0, double height = 0.0)
        : x(x), y(y), width(width), height(height) {}
    
    Rectangle clone() const { 
        return Rectangle(x, y, width, height); 
    }
    
    double getLeft() const { return x; }
    double getRight() const { return x + width; }
    double getTop() const { return y; }
    double getBottom() const { return y + height; }
    
    Point getTopLeft() const { return Point(x, y); }
    Point getBottomRight() const { return Point(x + width, y + height); }
    Point getSize() const { return Point(width, height); }
    
    bool contains(double px, double py) const {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }
    
    bool containsPoint(const Point& point) const {
        return contains(point.x, point.y);
    }
    
    bool containsRect(const Rectangle& rect) const {
        return rect.x >= x && rect.y >= y && 
               rect.x + rect.width <= x + width && 
               rect.y + rect.height <= y + height;
    }
    
    bool intersects(const Rectangle& other) const {
        return !(other.x > x + width || other.x + other.width < x ||
                other.y > y + height || other.y + other.height < y);
    }
    
    Rectangle intersection(const Rectangle& other) const {
        double left = std::max(x, other.x);
        double top = std::max(y, other.y);
        double right = std::min(x + width, other.x + other.width);
        double bottom = std::min(y + height, other.y + other.height);
        
        if (left < right && top < bottom) {
            return Rectangle(left, top, right - left, bottom - top);
        }
        return Rectangle();
    }
    
    Rectangle unite(const Rectangle& other) const {
        double left = std::min(x, other.x);
        double top = std::min(y, other.y);
        double right = std::max(x + width, other.x + other.width);
        double bottom = std::max(y + height, other.y + other.height);
        
        return Rectangle(left, top, right - left, bottom - top);
    }
    
    void inflate(double dx, double dy) {
        x -= dx;
        y -= dy;
        width += 2 * dx;
        height += 2 * dy;
    }
    
    void inflatePoint(const Point& point) {
        inflate(point.x, point.y);
    }
    
    void offset(double dx, double dy) {
        x += dx;
        y += dy;
    }
    
    void offsetPoint(const Point& point) {
        offset(point.x, point.y);
    }
    
    bool isEmpty() const {
        return width <= 0 || height <= 0;
    }
    
    void setEmpty() {
        x = y = width = height = 0.0;
    }
    
    bool equals(const Rectangle& other) const {
        return x == other.x && y == other.y && 
               width == other.width && height == other.height;
    }
    
    std::string toString() const {
        return "(x=" + std::to_string(x) + ", y=" + std::to_string(y) + 
               ", w=" + std::to_string(width) + ", h=" + std::to_string(height) + ")";
    }
};

/**
 * Matrix class equivalent to ActionScript's flash.geom.Matrix
 */
class Matrix {
public:
    double a = 1.0;  // scale x
    double b = 0.0;  // skew x
    double c = 0.0;  // skew y
    double d = 1.0;  // scale y
    double tx = 0.0; // translate x
    double ty = 0.0; // translate y
    
    Matrix(double a = 1.0, double b = 0.0, double c = 0.0, 
           double d = 1.0, double tx = 0.0, double ty = 0.0)
        : a(a), b(b), c(c), d(d), tx(tx), ty(ty) {}
    
    Matrix clone() const {
        return Matrix(a, b, c, d, tx, ty);
    }
    
    void identity() {
        a = d = 1.0;
        b = c = tx = ty = 0.0;
    }
    
    void invert() {
        double det = a * d - b * c;
        if (det != 0) {
            double na = d / det;
            double nb = -b / det;
            double nc = -c / det;
            double nd = a / det;
            double ntx = (c * ty - d * tx) / det;
            double nty = (b * tx - a * ty) / det;
            
            a = na; b = nb; c = nc; d = nd; tx = ntx; ty = nty;
        }
    }
    
    void rotate(double angle) {
        double cos_val = std::cos(angle);
        double sin_val = std::sin(angle);
        
        double na = a * cos_val - b * sin_val;
        double nb = a * sin_val + b * cos_val;
        double nc = c * cos_val - d * sin_val;
        double nd = c * sin_val + d * cos_val;
        
        a = na; b = nb; c = nc; d = nd;
    }
    
    void scale(double sx, double sy) {
        a *= sx;
        b *= sy;
        c *= sx;
        d *= sy;
        tx *= sx;
        ty *= sy;
    }
    
    void translate(double dx, double dy) {
        tx += dx;
        ty += dy;
    }
    
    Point transformPoint(const Point& point) const {
        return Point(
            a * point.x + c * point.y + tx,
            b * point.x + d * point.y + ty
        );
    }
    
    Point deltaTransformPoint(const Point& point) const {
        return Point(
            a * point.x + c * point.y,
            b * point.x + d * point.y
        );
    }
    
    void concat(const Matrix& other) {
        double na = a * other.a + b * other.c;
        double nb = a * other.b + b * other.d;
        double nc = c * other.a + d * other.c;
        double nd = c * other.b + d * other.d;
        double ntx = tx * other.a + ty * other.c + other.tx;
        double nty = tx * other.b + ty * other.d + other.ty;
        
        a = na; b = nb; c = nc; d = nd; tx = ntx; ty = nty;
    }
    
    std::string toString() const {
        return "(a=" + std::to_string(a) + ", b=" + std::to_string(b) + 
               ", c=" + std::to_string(c) + ", d=" + std::to_string(d) + 
               ", tx=" + std::to_string(tx) + ", ty=" + std::to_string(ty) + ")";
    }
};

} // namespace geom
} // namespace flash