#include "game.h"
#include "gameObject.h"
#include "common.h"
#include <GL/glut.h>

vector<definition> definitions;

void init(){

    // player
    definition player;
    player.bodyDef.type = b2_dynamicBody;
    player.shapeDef.density = 1.0f;
    player.shapeDef.filter.categoryBits = PLAYER;
    player.shapeDef.filter.maskBits = PLAYER_MASK;
    static b2Polygon playerBox = b2MakeOffsetBox(PLAYER_SIZE / 2, PLAYER_SIZE / 2, {0,0}, b2Rot_identity);
    player.shape = (ShapeUnion*)&playerBox;
    player.shapeCount = 1;
    definitions.push_back(player);

    // ground
    definition ground;
    ground.bodyDef.type = b2_dynamicBody;
    ground.shapeDef.density = 1.0f;
    ground.shapeDef.filter.categoryBits = GROUND;
    ground.shapeDef.filter.maskBits = GROUND_MASK;
    static b2Polygon groundBox = b2MakeOffsetBox(GROUND_SIZE / 2, GROUND_SIZE / 2, {0,0}, b2Rot_identity);
    ground.shape = (ShapeUnion*)&groundBox;
    definitions.push_back(ground);

    // water
    definition water;
    water.bodyDef.type = b2_dynamicBody;
    water.shapeDef.density = 1.0f;
    water.shapeDef.filter.categoryBits = WATER;
    water.shapeDef.filter.maskBits = WATER_MASK;
    static b2Polygon waterBox = b2MakeOffsetBox(WATER_SIZE / 2, WATER_SIZE / 2, {0,0}, b2Rot_identity);
    water.shape = (ShapeUnion*)&waterBox;
    water.shapeCount = 1;
    definitions.push_back(water);

    // portal
    definition portal;
    // Body is static by default
    portal.shapeDef.density = 1.0f;
    portal.shapeDef.filter.categoryBits = PORTAL;
    portal.shapeDef.filter.maskBits = PORTAL_MASK;
    portal.shape = (ShapeUnion*)&portalPolygon; 
    definitions.push_back(portal);
    
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f); // Bottom-left
    glVertex2f(0.5f, -0.5f);  // Bottom-right
    glVertex2f(0.5f, 0.5f);   // Top-right
    glVertex2f(-0.5f, 0.5f);  // Top-left
    glEnd();
    glFlush();
}

int main (int argc, char** argv) 
{
    init();
    glutInit(&argc, argv); // Use the original `argc` and `argv`
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Red Square");
    glutDisplayFunc(display);
    glutMainLoop();
}
