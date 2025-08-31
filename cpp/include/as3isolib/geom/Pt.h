#pragma once

#include "flash/geom/Point.h"
#include <cmath>
#include <string>

namespace as3isolib {
namespace geom {

/**
 * Pt class equivalent to ActionScript's as3isolib.geom.Pt
 * Extends flash.geom.Point with a z coordinate for 3D points
 */
class Pt : public flash::geom::Point {
public:
    double z = 0.0;
    
    Pt(double x = 0.0, double y = 0.0, double z = 0.0) 
        : flash::geom::Point(x, y), z(z) {}
    
    virtual ~Pt() = default;
    
    // Static calculation methods
    static double distance(const Pt& ptA, const Pt& ptB) {
        double tx = ptB.x - ptA.x;
        double ty = ptB.y - ptA.y;
        double tz = ptB.z - ptA.z;
        return std::sqrt(tx * tx + ty * ty + tz * tz);
    }
    
    static double theta(const Pt& ptA, const Pt& ptB) {
        double tx = ptB.x - ptA.x;
        double ty = ptB.y - ptA.y;
        
        double radians = std::atan(ty / tx);
        if (tx < 0) {
            radians += M_PI;
        }
        if (tx >= 0 && ty < 0) {
            radians += M_PI * 2;
        }
        return radians;
    }
    
    static double angle(const Pt& ptA, const Pt& ptB) {
        return theta(ptA, ptB) * 180.0 / M_PI;
    }
    
    static Pt polar(const Pt& originPt, double radius, double theta = 0.0) {
        double tx = originPt.x + std::cos(theta) * radius;
        double ty = originPt.y + std::sin(theta) * radius;
        double tz = originPt.z;
        return Pt(tx, ty, tz);
    }
    
    static Pt interpolate(const Pt& ptA, const Pt& ptB, double f) {
        if (f <= 0) return ptA;
        if (f >= 1) return ptB;
        
        double nx = (ptB.x - ptA.x) * f + ptA.x;
        double ny = (ptB.y - ptA.y) * f + ptA.y;
        double nz = (ptB.z - ptA.z) * f + ptA.z;
        
        return Pt(nx, ny, nz);
    }
    
    // Override length to include z coordinate
    double getLength() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    // Override clone to return Pt
    Pt clonePt() const {
        return Pt(x, y, z);
    }
    
    // Flash Point compatibility
    flash::geom::Point cloneAsPoint() const {
        return flash::geom::Point(x, y);
    }
    
    // String representation
    std::string toString() const {
        return "x:" + std::to_string(x) + " y:" + std::to_string(y) + " z:" + std::to_string(z);
    }
    
    // Operators for convenience
    Pt operator+(const Pt& other) const {
        return Pt(x + other.x, y + other.y, z + other.z);
    }
    
    Pt operator-(const Pt& other) const {
        return Pt(x - other.x, y - other.y, z - other.z);
    }
    
    Pt operator*(double scalar) const {
        return Pt(x * scalar, y * scalar, z * scalar);
    }
    
    Pt operator/(double scalar) const {
        return Pt(x / scalar, y / scalar, z / scalar);
    }
    
    bool operator==(const Pt& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Pt& other) const {
        return !(*this == other);
    }
    
    // Assignment operators
    Pt& operator+=(const Pt& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Pt& operator-=(const Pt& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    Pt& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    Pt& operator/=(double scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
};

} // namespace geom
} // namespace as3isolib