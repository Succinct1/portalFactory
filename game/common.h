#ifndef COMMON_H
#define COMMON_H

#define CHUNK_SIZE 256

#define WORLD_SLOPE 0.5f // How much the y changes per x
#define SLOPE_SNAPBACK 0.5f // How much generation will snap back to the slope

// Store leftmost and rightmost points of slope for choosing next
// Chance next chunk will continue is 1 - ((1 + count) / (base + count))

// Areas:

// Lake
#define LAKE_LENGTH 100
#define LAKE_LENGTH_VARIANCE 20.0f
#define LAKE_HEIGHT 10
#define LAKE_HEIGHT_VARIANCE 4.0f
#define LAKE_EXTEND_BASE 5
#define LAKE_EXTEND_COUNT 1
// Additional parameters needed for bottom of lake

// Plateu is one slope
#define PLATEU_MAX_HEIGHT 8
#define PLATEU_MIN_HEIGHT 0
#define PLATEU_HEIGHT_RANGE PLATEU_MAX_HEIGHT - PLATEU_MIN_HEIGHT
#define PLATEU_EXTEND_BASE 5
#define PLATEU_EXTEND_COUNT 1

// Slide is sction of constant slope that brings us back to the world slope
// delta is how far off the start of the chunk is from the world slope
// Snapback = delta * SLOPE_SNAPBACK
// slope = 
// length = (SLIDE_LENGTH + SLIDE_LENGHT_VARIANCE)
// height = ((WORLD_SLOPE + delta) * length + SLIDE_HEIGHT_VARIANCE)
#define SLIDE_MAX_SLOPE WORLD_SLOPE * 2.0f
#define SLIDE_MIN_SLOPE WORLD_SLOPE * 0.5f
#define SLIDE_SLOPE_VARIANCE WORLD_SLOPE * 0.5f

#define SLIDE_MIN_LENGTH 8
#define SLIDE_LENGHT_VARIANCE 2.0f
#define SLIDE_MAX_HEIGHT 16
#define SLIDE_MIN_HEIGHT 4


// Jagged is inconstant slope
#define JAGGED_MAX_HEIGHT 32
#define JAGGED_MIN_HEIGHT -4
#define JAGGED_HEIGHT_VARIANCE 0.5f 
#define JAGGED_LENGTH 4
#define JAGGED_LENGHT_VARIANCE 2.0f
// Steep

// Cliff and cave

// False Peak / sinkhole / underground cave

#define PLAYER_SIZE 10
#define WATER_SIZE 1
#define GROUND_SIZE 1

#define PORTAL_DEPTH 10
#define PORTAL_INTERNAL_WIDTH 50
#define PORTAL_WALL_WIDTH 5

#define PORTAL_HALF_HEIGHT (PORTAL_DEPTH + PORTAL_WALL_WIDTH) / 2
#define PORTAL_HALF_WIDTH (PORTAL_INTERNAL_WIDTH + PORTAL_WALL_WIDTH) / 2

#include "box2d/box2d.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <random>

using namespace std;

union ShapeUnion 
{
    b2Capsule capsule;
    b2Circle circle;
    b2Polygon polygon;
    b2Segment segment;
    b2ChainSegment chainSegment;
};

struct definition
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    ShapeUnion* shape;
    int shapeCount;
};

enum definitionIndex
{
    PLAYER_DEF,
    GROUND_DEF,
    WATER_DEF
};

enum slopeType
{
    LAKE,
    PLATEU,
    SLIDE,
    JAGGED,
    STEEP,
    CLIFF
};

// Change this as the types are overlapping
enum objectCategory{
    PLAYER = 0x00000001,
    GROUND = 0x00000002,
    WATER = 0x00000004,
    PORTAL = 0x00000008,
    VOID = 0x00000010,
    CELING = 0x00000020,

    PLAYER_MASK = PLAYER | GROUND | WATER | PORTAL | VOID | CELING,
    GROUND_MASK = PLAYER | GROUND | WATER | PORTAL | VOID | CELING,
    WATER_MASK = PLAYER | GROUND | WATER | PORTAL | VOID | CELING,
    PORTAL_MASK = PLAYER | GROUND | WATER | PORTAL | VOID | CELING,
    VOID_MASK = PLAYER | GROUND | WATER | PORTAL | VOID | CELING,
    CELING_MASK = PLAYER | GROUND | WATER | PORTAL | VOID | CELING,
};

// Makes a U shape starting from the top left corner and going ccw
b2Vec2 portalVerticies[8] = {
    {-PORTAL_HALF_WIDTH,  PORTAL_HALF_HEIGHT},
    {-PORTAL_HALF_WIDTH, -PORTAL_HALF_HEIGHT},
    { PORTAL_HALF_WIDTH, -PORTAL_HALF_HEIGHT},
    { PORTAL_HALF_WIDTH,  PORTAL_HALF_HEIGHT},
    { PORTAL_HALF_WIDTH - PORTAL_WALL_WIDTH, PORTAL_HALF_HEIGHT},
    { PORTAL_HALF_WIDTH - PORTAL_WALL_WIDTH, PORTAL_HALF_HEIGHT - PORTAL_DEPTH},
    {-PORTAL_HALF_WIDTH + PORTAL_WALL_WIDTH, PORTAL_HALF_HEIGHT - PORTAL_DEPTH},
    {-PORTAL_HALF_WIDTH + PORTAL_WALL_WIDTH, PORTAL_HALF_HEIGHT}
};

static b2Hull hull = b2ComputeHull(portalVerticies, 8);
static b2Polygon portalPolygon = b2MakePolygon(&hull, 0);

struct b2ShapeIdHash {
    std::size_t operator()(const b2ShapeId& id) const {
        std::hash<int32_t> hashIndex;
        std::hash<uint16_t> hashWorld;
        std::hash<uint16_t> hashGeneration;
        return hashIndex(id.index1) ^ (hashWorld(id.world0) << 1) ^ (hashGeneration(id.generation) << 2);
    }
};

struct b2ShapeIdEqual {
    bool operator()(const b2ShapeId& lhs, const b2ShapeId& rhs) const {
        return lhs.index1 == rhs.index1 && lhs.world0 == rhs.world0 && lhs.generation == rhs.generation;
    }
};

#endif // COMMON_H