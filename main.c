#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define WIDTH 1000
#define HEIGHT 1000
#define ROWS 10
#define COLS 10

const Color translucentWhite = {255, 255, 255, 20};
int cellWidth = WIDTH / ROWS;
int cellHeight = HEIGHT / ROWS;

Vector2 screenToCartesian(Vector2 pos) {
    double cartesianX = pos.x - WIDTH / 2.0f;
    double cartesianY = HEIGHT / 2.0F - pos.y;
    Vector2 cartesianCoord = {cartesianX, cartesianY};
    return cartesianCoord;
}

Vector2 cartesianToScreen(Vector2 pos) {
    double screenX = pos.x + WIDTH / 2.0f;
    double screenY = HEIGHT / 2.0f - pos.y;
    Vector2 screenCoord = {screenX, screenY};
    return screenCoord;
}

typedef struct {
    Vector2 startPos;
    Vector2 endPos;
} Wall;

typedef struct {
    Vector2 pos;
    Vector2 direction;
} myRay;

typedef struct {
    bool hit;
    Vector2 point;
} raycastResult;

void myRayDraw(myRay ray) { DrawLineV(ray.pos, ray.direction, translucentWhite); }

// Line-line intersection (https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection)
raycastResult raycast(myRay ray, Wall wall) {
    int x1 = wall.startPos.x; 
    int y1 = wall.startPos.y; 
    int x2 = wall.endPos.x; 
    int y2 = wall.endPos.y; 

    int x3 = ray.pos.x;
    int y3 = ray.pos.y;
    int x4 = ray.direction.x;
    int y4 = ray.direction.y;

    double den = (x1 - x2) * (y3 - y4) - (y1 - y1) * (x3 * x4);

    raycastResult result;
    if (den == 0) {
        result.hit = 0;
        return result;
    }
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 * x4)) / den;
    double u = -((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 * x2)) / den;
    
    if (0 > t < 1 && u > 0)
        result.hit = true;
    else
        result.hit = false;

    return result;
}
bool inBounds(Vector2 pos) {
    return pos.x >= 0 && pos.x < WIDTH && pos.y >= 0 && pos.y < HEIGHT;
}

void drawWall(Wall wall) {
    DrawLineV(wall.startPos, wall.endPos, WHITE);
}

double degreesToRadians(double degree) {
    return degree * PI / 180;
}

int main() {
    srand(time(0));
    InitWindow(WIDTH, HEIGHT, "Raycaster");
    SetTargetFPS(60);
    
    Vector2 startPos = {200, 300};
    Vector2 endPos = {400, 500};
    Wall wall1 = {startPos, endPos};
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        drawWall(wall1);
        for (int d = 0; d < 360; d++) {
            Vector2 rayOrigin = {GetMouseX(), GetMouseY()};
            rayOrigin = screenToCartesian(rayOrigin);
            Vector2 rayDirection = {cos(degreesToRadians(d)) * WIDTH, sin(degreesToRadians(d)) * HEIGHT};
            myRay ray = {cartesianToScreen(rayOrigin), cartesianToScreen(rayDirection)};
            myRayDraw(ray);
        }
        EndDrawing();
    }
}
