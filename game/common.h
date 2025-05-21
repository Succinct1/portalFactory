#ifndef COMMON_H
#define COMMON_H

#define DEFAULT_TIME_STEP 1.0f / 60.0f

#define VIEW_WIDTH 4000
#define VIEW_HEIGHT 2000

#define CHUNK_WIDTH INT16_MAX // Number of chunks wide the world can be
#define CHUNK_MAX_HEIGHT 64 // Number of chunks high the world can be
#define CHUNK_MIN_HEIGHT INT16_MIN // Minimum chunk coordinate
#define CHUNK_SIZE 256
#define CHUNK_MAX_INDEX CHUNK_SIZE * CHUNK_SIZE - 1

#define WORLD_SLOPE 0.5f // How much the y changes per x

// delta is how far off the start of the chunk is from the world slope
// Two kinds of areas, corrective and non-corrective based on delta sign
// Chance for a  non-corrective slope is (1 + k * delta) / (base + k * delta)
#define SNAPBACK_BASE 2.0f
#define SNAPBACK_MODIFIER 0.5f // Lower value of k means less correction

// Store leftmost and rightmost points of slope for choosing next
// Chance next chunk will continue is 1 - ((1 + count) / (base + count))

// Areas:

// Lake can be at any position in the chunk with world slope before and after
#define LAKE_LENGTH 100
#define LAKE_LENGTH_VARIANCE 20.0f
#define LAKE_DEPTH 10
#define LAKE_DEPTH_VARIANCE 4.0f
// Lake extensions
#define LAKE_EXTEND_BASE 5
#define LAKE_EXTEND_COUNT 1
// Additional parameters needed for bottom of lake

// Plateu is one slope guarenteed to end on x chunk boundary
#define PLATEU_MAX_HEIGHT 8
#define PLATEU_MIN_HEIGHT 0
#define PLATEU_HEIGHT_RANGE PLATEU_MAX_HEIGHT - PLATEU_MIN_HEIGHT
#define PLATEU_EXTEND_BASE 5
#define PLATEU_EXTEND_COUNT 1

// Slide is sction of constant slope that brings us closer to the world slope
// 1. pick a slope based on 1 chunk of correction to world slope.
// slope = (delta + CHUNK_SIZE * WORLD_SLOPE) / CHUNK_SIZE +- SLIDE_SLOPE_VARIANCE
// 2. Snap to range
#define SLIDE_MAX_SLOPE WORLD_SLOPE * 2.0f
#define SLIDE_MIN_SLOPE WORLD_SLOPE * 0.5f
#define SLIDE_SLOPE_VARIANCE WORLD_SLOPE * 0.25f
// 3. find out how much we will drop in this chunk
// 4. break the slope into sections of length SLIDE_MIN_LENGTH + SLIDE_LENGHT_VARIANCE
// 5. height = (slope * length + (delta in chunk)) +- SLIDE_HEIGHT_VARIANCE
#define SLIDE_MIN_LENGTH 8
#define SLIDE_LENGTH_VARIANCE 2.0f
#define SLIDE_HEIGHT_VARIANCE 2.0f
#define SLIDE_EXTEND_BASE 5
#define SLIDE_EXTEND_COUNT 1

// Jagged is slope with more variance
#define JAGGED_MAX_SLOPE WORLD_SLOPE * 4.0f
#define JAGGED_MIN_SLOPE WORLD_SLOPE * 0.25f
#define JAGGED_SLOPE_VARIANCE WORLD_SLOPE * 0.25f
#define JAGGED_MIN_LENGTH 4
#define JAGGED_LENGTH_VARIANCE 2.0f
#define JAGGED_HEIGHT_VARIANCE 4.0f
#define JAGGED_EXTEND_BASE 5
#define JAGGED_EXTEND_COUNT 1

// Steep has one slope
#define STEEP_MAX_SLOPE WORLD_SLOPE * 8.0f
#define STEEP_MIN_SLOPE WORLD_SLOPE * 2.0f
#define STEEP_SLOPE_VARIANCE WORLD_SLOPE * 1.0f
#define STEEP_EXTEND_BASE 5
#define STEEP_EXTEND_COUNT 1

// Cliff and cave
#define CLIFF_MIN_HEIGHT 20.0f
#define CLIFF_HEIGHT_VARIANCE 20.0f
#define CAVE_DEPTH 5.0f
#define CAVE_DEPTH_VARIANCE 10.0f
// World slope for length to next
#define BASE_LENGTH 5.0f
#define BASE_LENGTH_VARIANCE 2.0f
// After last cliff, plateu or lake to edge of chunk
#define CLIFF_EXTEND_BASE 5
#define CLIFF_EXTEND_COUNT 1

// False Peak / sinkhole / underground cave
// Water drains into cave the exits in a cliff section lower down

#define PLAYER_SIZE 10
#define WATER_SIZE 1
#define GROUND_SIZE 1 // size of a block in the ground

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
    WATER_DEF,
    PORTAL_DEF
};

enum chunkCategory
{
    AIR = 'a',
    SURFACE = 's',
    GROUND = 'g'
};

enum slopeTypes
{
    LAKE,
    PLATEU,
    SLIDE,
    JAGGED,
    STEEP,
    CLIFF,
    SLOPE_TYPES_COUNT
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