#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define WIDTH 1000
#define HEIGHT 1000

const Color translucentWhite = {255, 255, 255, 50};
const Color rayHitColor = {255, 0, 0, 50};

typedef struct {
    Vector2 startPos;
    Vector2 endPos;
} Wall;

typedef struct {
    Vector2 pos;
    Vector2 dir;
} myRay;

typedef struct {
    bool hit;
    Vector2 point;
} raycastResult;

void myRayDraw(myRay ray, Vector2 point, bool hit) { 
    double len = sqrtf(WIDTH * WIDTH + HEIGHT * HEIGHT);
    if (hit) DrawLineV(ray.pos, point, rayHitColor); 
    else DrawLineV(ray.pos, Vector2Add(ray.pos, Vector2Scale(point, len)), translucentWhite); 
}

// Line-line intersection (https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection)
raycastResult raycast(myRay ray, Wall wall) {
    double len = sqrtf(WIDTH * WIDTH + HEIGHT * HEIGHT);
    int x1 = wall.startPos.x;
    int y1 = wall.startPos.y; 
    int x2 = wall.endPos.x; 
    int y2 = wall.endPos.y; 

    int x3 = ray.pos.x;
    int y3 = ray.pos.y;
    int x4 = ray.dir.x * len;
    int y4 = ray.dir.y * len;

    double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    raycastResult result;

    if (denominator == 0) {
        result.hit = 0;
        result.point = ray.dir;
        return result;
    }
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
    double u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / denominator;
    
    if (t > 0 && t < 1 && u < 0) {
        result.hit = true;
        result.point.x = x1 + t * (x2 - x1);
        result.point.y = y1 + t * (y2 - y1);
        return result;
    }
    else {
        result.hit = false;
        return result;
    }

}

void drawWall(Wall wall) {
    DrawLineV(wall.startPos, wall.endPos, BLUE);
}

Vector2 GetMousePositionInWorld() {
    Vector2 p = GetMousePosition();
	p.x -= WIDTH / 2.0f;
	p.y -= HEIGHT / 2.0f;
	p.y *= -1;

	return p;
}


int main() {
    Vector2 startPos = {0, 200};
    Vector2 endPos = {300, 200};
    Wall wall1 = {startPos, endPos};
    Vector2 startPos2 = {10, 300};
    Vector2 endPos2 = {-100, 400};
    Wall wall2 = {startPos2, endPos2};
    Wall walls[10] = {wall1, wall2};

    InitWindow(WIDTH, HEIGHT, "Raycaster");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        rlPushMatrix();
		rlTranslatef(WIDTH / 2.0f, HEIGHT / 2.0f, 0);
		rlScalef(1, -1, 1);
		rlDisableBackfaceCulling();

        Vector2 rayPos = GetMousePositionInWorld();
        for (int d = 0; d < 360; d++) {
            Vector2 raydir = {cosf(d * DEG2RAD), sinf(d * DEG2RAD)};
            myRay ray = {rayPos, raydir};
            for (int i = 0; i < 10; i++) {
                raycastResult result = raycast(ray, walls[i]);
                myRayDraw(ray, result.point, result.hit);
            }
        }
        for (int i = 0; i < 10; i++)
            drawWall(walls[i]);

        rlDrawRenderBatchActive();
		rlEnableBackfaceCulling();
		rlPopMatrix();
        DrawFPS(0, 0);
        EndDrawing();
    }
}
