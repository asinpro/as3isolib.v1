#include <catch2/catch_test_macros.hpp>
#include "as3isolib/geom/Pt.h"
#include <cmath>

using namespace as3isolib::geom;

TEST_CASE("Pt basic functionality", "[Pt]") {
    SECTION("Constructor and basic properties") {
        Pt pt(1.0, 2.0, 3.0);
        REQUIRE(pt.x == 1.0);
        REQUIRE(pt.y == 2.0);
        REQUIRE(pt.z == 3.0);
    }
    
    SECTION("Default constructor") {
        Pt pt;
        REQUIRE(pt.x == 0.0);
        REQUIRE(pt.y == 0.0);
        REQUIRE(pt.z == 0.0);
    }
    
    SECTION("Length calculation with z coordinate") {
        Pt pt(3.0, 4.0, 12.0);
        double expected = std::sqrt(3*3 + 4*4 + 12*12); // Should be 13
        REQUIRE(pt.getLength() == expected);
    }
    
    SECTION("Clone functionality") {
        Pt pt(1.5, 2.5, 3.5);
        Pt cloned = pt.clonePt();
        REQUIRE(cloned.x == pt.x);
        REQUIRE(cloned.y == pt.y);
        REQUIRE(cloned.z == pt.z);
    }
}

TEST_CASE("Pt static methods", "[Pt]") {
    SECTION("Distance calculation") {
        Pt pt1(0.0, 0.0, 0.0);
        Pt pt2(3.0, 4.0, 12.0);
        double distance = Pt::distance(pt1, pt2);
        REQUIRE(distance == 13.0);
    }
    
    SECTION("Angle calculation") {
        Pt pt1(0.0, 0.0, 0.0);
        Pt pt2(1.0, 1.0, 0.0);
        double angle = Pt::angle(pt1, pt2);
        REQUIRE(angle == 45.0);
    }
    
    SECTION("Interpolation") {
        Pt pt1(0.0, 0.0, 0.0);
        Pt pt2(10.0, 10.0, 10.0);
        
        Pt mid = Pt::interpolate(pt1, pt2, 0.5);
        REQUIRE(mid.x == 5.0);
        REQUIRE(mid.y == 5.0);
        REQUIRE(mid.z == 5.0);
        
        Pt start = Pt::interpolate(pt1, pt2, 0.0);
        REQUIRE(start == pt1);
        
        Pt end = Pt::interpolate(pt1, pt2, 1.0);
        REQUIRE(end == pt2);
    }
    
    SECTION("Polar coordinates") {
        Pt origin(0.0, 0.0, 5.0);
        Pt polar = Pt::polar(origin, 1.0, 0.0); // 1 unit at 0 radians
        REQUIRE(polar.x == 1.0);
        REQUIRE(polar.y == 0.0);
        REQUIRE(polar.z == 5.0); // Z should be preserved
    }
}

TEST_CASE("Pt operators", "[Pt]") {
    SECTION("Addition") {
        Pt pt1(1.0, 2.0, 3.0);
        Pt pt2(4.0, 5.0, 6.0);
        Pt result = pt1 + pt2;
        REQUIRE(result.x == 5.0);
        REQUIRE(result.y == 7.0);
        REQUIRE(result.z == 9.0);
    }
    
    SECTION("Subtraction") {
        Pt pt1(10.0, 8.0, 6.0);
        Pt pt2(4.0, 3.0, 2.0);
        Pt result = pt1 - pt2;
        REQUIRE(result.x == 6.0);
        REQUIRE(result.y == 5.0);
        REQUIRE(result.z == 4.0);
    }
    
    SECTION("Scalar multiplication") {
        Pt pt(2.0, 3.0, 4.0);
        Pt result = pt * 2.0;
        REQUIRE(result.x == 4.0);
        REQUIRE(result.y == 6.0);
        REQUIRE(result.z == 8.0);
    }
    
    SECTION("Equality") {
        Pt pt1(1.0, 2.0, 3.0);
        Pt pt2(1.0, 2.0, 3.0);
        Pt pt3(1.0, 2.0, 4.0);
        
        REQUIRE(pt1 == pt2);
        REQUIRE(pt1 != pt3);
    }
}