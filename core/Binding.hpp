#pragma once

#include "Common.hpp"
#include <set>
#include "Tilemap.hpp"

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

// Binding configuration
class Binding {
public:
    // utilities
    static bool CheckLua(lua_State*, int);
    static bool CheckEntityExist(Entity);
    static void DestroyEntityOnTileMap(Entity);
    static void DispatchTileMapChangedData(const Tilemap &);

    // entity-related methods
    static int lua_CreateEntity(lua_State*);
    static int lua_DestroyEntity(lua_State*);

    // get methods
    static int lua_GetEntity(lua_State*);
    static int lua_GetEntityByName(lua_State*);

    // data types
    // Vec2D
    static int Vec2DIndex(lua_State*);
    static int Vec2DNewIndex(lua_State*);
    static int Vec2DGC(lua_State*);
    static void RegisterVec2DComponentMetatable(lua_State*);

    // components
    // Transform component
    static int lua_GetTransformComponent(lua_State*);
    static int TransformIndex(lua_State*);
    static int TransformGC(lua_State*);
    static void RegisterTransformComponentMetatable(lua_State*);

    // Name component
    static int lua_SetNameComponent(lua_State*);
    static int lua_RemoveNameComponent(lua_State*);

    // Tag Component
    static int lua_GetTagComponent(lua_State*);
    static int lua_SetTagComponent(lua_State*);
    static int lua_RemoveTagComponent(lua_State*);

    // Script component (Actually get the script path)
    static int lua_GetScriptComponent(lua_State*);

    // Physics
    static int lua_AddB2Body(lua_State*);
    static int lua_RemoveB2Body(lua_State*);
    static int lua_SetVelocity(lua_State*);
    static int lua_AddForce(lua_State*);
    static int lua_CheckCollision(lua_State*);
    static int lua_OnCollision(lua_State*);

    // Get Input
    static int lua_GetHorizontalInput(lua_State*);
    static int lua_GetVerticalInput(lua_State*);
    static int lua_GetSpaceInput(lua_State*);

    // Gameplay
    static int lua_Win(lua_State*);

    // entity list
    static set<Entity> *entities;
    // the entity in process
    static Entity curEntity;
    // Lua Stack
    static lua_State *L;
};