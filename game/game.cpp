#include "game.h"

game::game() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 1.0f};
    worldDef.maximumLinearSpeed = 5000.0f;

    worldId = b2CreateWorld(&worldDef);
    worldSeed = rand(); // Initialize the world seed
}

game::game(string file) {
    // Load the world from the file
}

game::~game() {
}

void game::gameTick() {
    b2World_Step(worldId, timeStep, 0);

    // Handle sensor events
    b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);
    
    // Start events occur when a shape starts touching a sensor
    // Shapes are guarenteed to be valid
    for (int i = 0; i < sensorEvents.beginCount; i++) {
        b2SensorBeginTouchEvent* event = &sensorEvents.beginEvents[i];
        gameObject* sensor = (gameObject*)b2Shape_GetUserData(event->sensorShapeId);
        sensor->sensorBeginTouch(event);
    }
    
    // End events occur when a shape stops touching a sensor or is destroyed
    // Not guaranteed to be valid
    for (int i = 0; i < sensorEvents.endCount; i++) {
        b2SensorEndTouchEvent* event = &sensorEvents.endEvents[i];
        if (!b2Shape_IsValid(event->sensorShapeId)) {
            continue; // Skip invalid shapes
        }
        gameObject* sensor = (gameObject*)b2Shape_GetUserData(event->sensorShapeId);
        sensor->sensorEndTouch(event);
    }

    b2ContactEvents contactEvents = b2World_GetContactEvents(worldId);
    
    // These events are generated when the aabb start overlapping though the manifold might 
    // be empty and the shape bodies might not be touching either.
    for (int i = 0; i < contactEvents.beginCount; i++) {
        b2ContactBeginTouchEvent* event = &contactEvents.beginEvents[i];
        gameObject* objA = (gameObject*)b2Shape_GetUserData(event->shapeIdA);
        gameObject* objB = (gameObject*)b2Shape_GetUserData(event->shapeIdB);
        objA->beginContact(event);
        objB->beginContact(event);
    }
    
    for (int i = 0; i < contactEvents.endCount; i++) {
        b2ContactEndTouchEvent* event = &contactEvents.endEvents[i];
        if (b2Shape_IsValid(event->shapeIdA)) {
            gameObject* objA = (gameObject*)b2Shape_GetUserData(event->shapeIdA);
            objA->endContact(event);
        }
        if (b2Shape_IsValid(event->shapeIdB)) {
            gameObject* objB = (gameObject*)b2Shape_GetUserData(event->shapeIdB);
            objB->endContact(event);
        }
    }

    // Update the game state
    for (auto& obj : updateList) {
        obj->update();
    }

    for (auto& obj : removeList) {
        obj->destroyThis();
    }
    removeList.clear();
}

void game::loadChunk(b2Vec2 coord) {
}

/// @brief Coord generates chunks until coord is covered
//         Will generate enought chunks on surface to determine type
/// @param coord 
void game::buildChunk(b2Vec2 coord) {
    // Get the chunk index
    int chunkIndex = getChunkIndex(coord);
    
    // Check if the chunk index is already in the map
    if (chunks.find(chunkIndex) != chunks.end()) {
        return; 
    }

    // Build surface chunks if needed to determine type
    if (coord.x < leftTerminus.x){
        while (coord.x < leftTerminus.x) {
            buildSurfaceChunk(true);
        }
    }
    else if (coord.x > rightTerminus.x){
        while (coord.x > rightTerminus.x) {
            buildSurfaceChunk(false);
        }
    }


}

void game::buildSurfaceChunk(bool left) {
    float delta, expected;
    int seed;
    if (left){
        expected = WORLD_SLOPE * leftTerminus.x;
        delta = leftTerminus.y - expected;
        seed = generateChunkSeed(leftTerminus);
    }
    else {
        expected = WORLD_SLOPE * rightTerminus.x;
        delta = rightTerminus.y - expected;
        seed = generateChunkSeed(rightTerminus);
    }

    float snapbackChance = (1 + SNAPBACK_MODIFIER * delta) / (SNAPBACK_BASE + SNAPBACK_MODIFIER * delta);

    std::mt19937 generator(seed);
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    float randomValue = distribution(generator);
    slopeTypes chosenSlope;

    if (delta < 0) {
        // Choose randomly between lake, plateu, slide, and jagged
        if (randomValue < snapbackChance) {
            chosenSlope = static_cast<slopeTypes>(generator() % 4); // LAKE to JAGGED
        } else {
            // Choose between steep and cliff
            chosenSlope = static_cast<slopeTypes>(STEEP + (generator() % 2)); // STEEP or CLIFF
        }
    } else {
        if (randomValue < snapbackChance) {
            // Choose between slide, jagged, steep, and cliff
            chosenSlope = static_cast<slopeTypes>(SLIDE + (generator() % 4)); // SLIDE to CLIFF
        }
        else {
            // Choose between lake, plateu, and jagged
            chosenSlope = static_cast<slopeTypes>(LAKE + (generator() % 2)); // LAKE or PLATEU
        }
    }

    randomValue = distribution(generator);
    switch (chosenSlope) {

        case LAKE:
            int count = 1;
            float length = LAKE_LENGTH + (distribution(generator) * LAKE_LENGTH_VARIANCE);
            float depth = LAKE_DEPTH + (distribution(generator) * LAKE_DEPTH_VARIANCE);
            while (randomValue > (1 + LAKE_EXTEND_COUNT * count) / (LAKE_EXTEND_BASE + LAKE_EXTEND_COUNT * count)) {
                length += LAKE_LENGTH + (distribution(generator) * LAKE_LENGTH_VARIANCE);
                depth += LAKE_DEPTH + (distribution(generator) * LAKE_DEPTH_VARIANCE);
                count++;
            }

            int chunkCount = length / CHUNK_SIZE + 1;

            b2Vec2 start;
            if (left) {
                length = -length;
                start = leftTerminus;
                for (int i = 1; i <= chunkCount; i++) {
                    addChunk(leftIndex - i);
                }

                if (depth > leftTerminus.y) {
                    ulong index = leftIndex - CHUNK_WIDTH;
                    for (int i = 1; i <= chunkCount; i++) {
                        addChunk(index - i);
                    }
                }
            }
            else {
                start = rightTerminus;
                for (int i = 1; i <= chunkCount; i++) {
                    addChunk(rightIndex + i);
                }

                if (depth > rightTerminus.y) {
                    ulong index = rightIndex - CHUNK_WIDTH;
                    for (int i = 1; i <= chunkCount; i++) {
                        addChunk(index - i);
                    }
                }
            }
            // addlines


            
            break;

        case PLATEU:
            // Generate a plateu
            break;
        case SLIDE:
            // Generate a slide
            break;
        case JAGGED:
            // Generate a jagged slope
            break;
        case STEEP:
            // Generate a steep slope
            break;
        case CLIFF:
            // Generate a cliff
            break;
        default:
            break;
    }
}

bool game::addChunk(uint index) {
    // Check if the chunk is already in the map
    if (chunks.find(index) != chunks.end()) {
        return false; // Chunk already exists
    }

    // Create a new chunk and add it to the map
    chunk* newChunk = new chunk();
    chunks[index] = newChunk;
    newChunk->seed = generateChunkSeed(getChunkCoord(index));
    newChunk->pos = getChunkCoord(index);
    newChunk->index = index;

    return true;
}

void game::getIds(b2Vec2 tl, b2Vec2 br, unordered_set<b2ShapeId>* collection) {
    b2AABB aabb;
    aabb.lowerBound = tl;
    aabb.upperBound = br;
    b2QueryFilter filter = b2DefaultQueryFilter();
    b2World_OverlapAABB(worldId, aabb, filter, getID, collection);
}

bool game::getID(b2ShapeId shapeId, void* context) {
    unordered_set<b2ShapeId>* collection = (unordered_set<b2ShapeId>*)context;
    collection->insert(shapeId);
    return true;
}

/// @brief Get box coordinates from index. Index starts at bottom left
/// @param index 
/// @param coord 
b2Vec2 game::getChunkCoord(ulong index) {
    b2Vec2 coord;
    coord.x = (index % CHUNK_WIDTH - CHUNK_WIDTH / 2) * CHUNK_SIZE;
    coord.y = (index / CHUNK_WIDTH - CHUNK_MIN_HEIGHT) * CHUNK_SIZE;
    return coord;
}

b2Vec2 game::getChunkCoord(b2Vec2 coord) {
    b2Vec2 chunkCoord;
    chunkCoord.x = floor(coord.x / CHUNK_SIZE) * CHUNK_SIZE;
    chunkCoord.y = floor(coord.y / CHUNK_SIZE) * CHUNK_SIZE;
    return chunkCoord;
}

/// @brief Gets index to store this in map
/// @param x 
/// @param y 
/// @return 
ulong game::getChunkIndex(b2Vec2 coord) {
    ulong index = (ulong)floor(coord.y / CHUNK_SIZE) * CHUNK_WIDTH + CHUNK_MIN_HEIGHT;
    return index + (ulong)floor(coord.x / CHUNK_SIZE) + CHUNK_WIDTH / 2;
}

int game::generateChunkSeed(b2Vec2 coord) {
    hash<int> hasher;
    return hasher(worldSeed) ^ (hasher((int)coord.x) << 1) ^ (hasher((int)coord.y) << 2);
}

b2Vec2 game::addLine(b2Vec2 start, int dx, int dy) {
    b2Vec2 end = start + b2Vec2{(float)(dx * GROUND_SIZE), (float)(dy * GROUND_SIZE)};
    int sx, sy, ex, ey;
    ulong sIndex = getChunkIndex(start);
    ulong eIndex = getChunkIndex(end);
    chunk* chunk = chunks[sIndex];
    
    // if (chunk == nullptr) {
        //     addChunk(index);
        //     chunk = chunks[index];
        // }
    if (sIndex != eIndex){

    }
    
    chunk->getCoord(start, &sx, &sy);
    while (sx < 0){
        sIndex--;
        chunk = chunks[sIndex];
        chunk->getCoord(start, &sx, &sy);
    }
    while (sy < 0){
        sIndex -= CHUNK_WIDTH;
        chunk = chunks[sIndex];
        chunk->getCoord(start, &sx, &sy);
    }
    end.x = start.x + dx * GROUND_SIZE;
    end.y = start.y + dy * GROUND_SIZE;

    if (dx > dy){
        float fRatio = (float)dx / (float)dy;
        while( sx >= 0 && sy >= 0 && sx < CHUNK_SIZE && sy < CHUNK_SIZE){
        }

    }
    

    return end;
}