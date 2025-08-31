#include <raylib.h>
#include "flash/display/DisplayObject.h"
#include "as3isolib/geom/Pt.h"
#include "as3isolib/events/IsoEvent.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace flash::display;
using namespace as3isolib::geom;
using namespace as3isolib::events;

// Simple isometric transformation functions
Pt isoToScreen(const Pt& isoPt) {
    // Standard isometric projection
    double screenX = (isoPt.x - isoPt.y) * 0.866; // cos(30°)
    double screenY = (isoPt.x + isoPt.y) * 0.5 - isoPt.z;
    return Pt(screenX, screenY, 0);
}

Pt screenToIso(const Pt& screenPt) {
    // Inverse isometric projection
    double isoX = (screenPt.x * 0.866 + screenPt.y) / 1.366;
    double isoY = (screenPt.y - screenPt.x * 0.866) / 1.366;
    return Pt(isoX, isoY, 0);
}

class IsoBox {
public:
    Pt position;
    Pt size;
    Color color;
    
    IsoBox(const Pt& pos, const Pt& sz, Color col)
        : position(pos), size(sz), color(col) {}
    
    void render(int offsetX, int offsetY) {
        // Convert isometric position to screen coordinates
        Pt screenPos = isoToScreen(position);
        Pt screenSize = isoToScreen(size);
        
        // Adjust for screen center
        int x = static_cast<int>(screenPos.x + offsetX);
        int y = static_cast<int>(screenPos.y + offsetY);
        int w = static_cast<int>(std::abs(screenSize.x));
        int h = static_cast<int>(std::abs(screenSize.y));
        
        // Draw the box as a simple rectangle
        DrawRectangle(x, y, w > 0 ? w : 20, h > 0 ? h : 20, color);
        
        // Draw outline
        DrawRectangleLines(x, y, w > 0 ? w : 20, h > 0 ? h : 20, BLACK);
    }
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "as3isolib C++ - Isometric Demo");
    SetTargetFPS(60);
    
    // Create some isometric objects
    std::vector<IsoBox> boxes;
    
    // Create a grid of boxes
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            for (int z = 0; z < 2; z++) {
                Color boxColor = {
                    static_cast<unsigned char>(100 + x * 30),
                    static_cast<unsigned char>(100 + y * 30),
                    static_cast<unsigned char>(100 + z * 50),
                    255
                };
                
                boxes.emplace_back(
                    Pt(x * 40.0, y * 40.0, z * 30.0),
                    Pt(35.0, 35.0, 25.0),
                    boxColor
                );
            }
        }
    }
    
    // Camera position
    double cameraX = 0.0;
    double cameraY = 0.0;
    
    // Test some 3D point operations
    Pt testPoint(100.0, 50.0, 25.0);
    std::cout << "Original 3D point: " << testPoint.toString() << std::endl;
    
    Pt screenPoint = isoToScreen(testPoint);
    std::cout << "Screen projection: " << screenPoint.toString() << std::endl;
    
    Pt backToIso = screenToIso(screenPoint);
    std::cout << "Back to isometric: " << backToIso.toString() << std::endl;
    
    // Test distance calculation
    Pt point1(0, 0, 0);
    Pt point2(30, 40, 50);
    double distance = Pt::distance(point1, point2);
    std::cout << "Distance between points: " << distance << std::endl;
    
    while (!WindowShouldClose()) {
        // Update camera with arrow keys
        if (IsKeyDown(KEY_LEFT)) cameraX -= 2.0;
        if (IsKeyDown(KEY_RIGHT)) cameraX += 2.0;
        if (IsKeyDown(KEY_UP)) cameraY -= 2.0;
        if (IsKeyDown(KEY_DOWN)) cameraY += 2.0;
        
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        
        // Calculate screen center with camera offset
        int centerX = screenWidth / 2 + static_cast<int>(cameraX);
        int centerY = screenHeight / 2 + static_cast<int>(cameraY);
        
        // Sort boxes by depth for proper rendering order (simple z-sorting)
        std::vector<size_t> indices;
        for (size_t i = 0; i < boxes.size(); i++) {
            indices.push_back(i);
        }
        
        std::sort(indices.begin(), indices.end(), [&boxes](size_t a, size_t b) {
            // Sort by Y then X then Z for proper isometric depth ordering
            const Pt& posA = boxes[a].position;
            const Pt& posB = boxes[b].position;
            if (posA.y != posB.y) return posA.y < posB.y;
            if (posA.x != posB.x) return posA.x < posB.x;
            return posA.z < posB.z;
        });
        
        // Render boxes in sorted order
        for (size_t idx : indices) {
            boxes[idx].render(centerX, centerY);
        }
        
        // Draw grid lines for reference
        DrawLine(centerX - 200, centerY, centerX + 200, centerY, DARKGRAY);
        DrawLine(centerX, centerY - 200, centerX, centerY + 200, DARKGRAY);
        
        // Draw UI
        DrawText("Isometric Demo - as3isolib C++", 10, 10, 20, DARKBLUE);
        DrawText("Use arrow keys to move camera", 10, 40, 12, DARKGRAY);
        DrawText("Press ESC to exit", 10, 60, 12, DARKGRAY);
        
        std::string infoText = "Camera: (" + std::to_string(static_cast<int>(cameraX)) + 
                              ", " + std::to_string(static_cast<int>(cameraY)) + ")";
        DrawText(infoText.c_str(), 10, screenHeight - 60, 12, DARKGRAY);
        
        std::string boxCount = "Boxes: " + std::to_string(boxes.size());
        DrawText(boxCount.c_str(), 10, screenHeight - 40, 12, DARKGRAY);
        
        std::string testInfo = "Test distance: " + std::to_string(distance);
        DrawText(testInfo.c_str(), 10, screenHeight - 20, 12, DARKGRAY);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}