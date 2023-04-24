#include "Binding.hpp"
#include "ECSCoordinator.hpp"
#include <algorithm>

using namespace std;

extern ECSCoordinator coordinator;

Entity Binding::curEntity;
lua_State* Binding::L = nullptr;
set<Entity>* Binding::entities = nullptr;

bool Binding::CheckLua(lua_State* L, int state) {
    if (state != LUA_OK) {
        // print error message
        // cout << lua_tostring(L, -1) << endl;
        return false;
    }
    return true;
}

bool Binding::CheckEntityExist(Entity entity) {
    if(entities->find(entity) != entities->end()) {
        return true;
    } else {
        return false;
    }
}

void Binding::DestroyEntityOnTileMap(Entity entity) {
    // auto& transform = coordinator.GetComponent<Transform>(entity);
    Tilemap& tilemap = coordinator.m_Tilemap;
    // int x = (int)(transform.position.x/(tilemap.m_TileWidth * tilemap.m_ScaleFactor));
    // int y = (int)(transform.position.x/(tilemap.m_TileHeight * tilemap.m_ScaleFactor));
    // int index = y * tilemap.m_MapX + x;
    // tilemap.m_Tiles[index] = -1;
    // // cout << "tilemap.m_Tiles[index]-------->"<< tilemap.m_Tiles[index];

    if(coordinator.HashComponent<Tile>(entity)) {
        int x = coordinator.GetComponent<Tile>(entity).index.x;
        int y = coordinator.GetComponent<Tile>(entity).index.y;
        tilemap.m_Tiles[y * tilemap.m_MapX + x] = -1;
        DispatchTileMapChangedData(tilemap);
    }

    
}

void Binding::DispatchTileMapChangedData(const Tilemap &tilemap)
{
    Event event(EVENT::EVENT_ON_TILEMAP_CHANGED);
    coordinator.TriggerEvent(event);
}

int Binding::lua_CreateEntity(lua_State* L) {
    // cout << "calling: [C++] CreateEntity(...)" << endl;
    Entity entity = coordinator.CreateEntity();
    // TODO: check valid
    lua_pushinteger(L, static_cast<lua_Integer>(entity));
    return 1;
}

int Binding::lua_DestroyEntity(lua_State* L) {
    // cout << "calling: [C++] DestroyEntity(...)" << endl;
    int paramNum = lua_gettop(L);
    // if(paramNum == 0){
    //     cout << "entering: paramNum == 0" << endl;
    //     //coordinator.DestroyEntity(Binding::curEntity);
    //     lua_pushboolean(L, !CheckEntityExist(Binding::curEntity));
    //     return 1;
    // } else 
    if(paramNum == 1) {
        // cout << "entering: paramNum == 1" << endl;
        Entity entity = static_cast<Entity>(lua_tointeger(L, 1));
        // cout << "passed in Entity: ----->" << entity << endl;
        if(coordinator.HashComponent<Tag>(entity)) {
            Tag& tag = coordinator.GetComponent<Tag>(entity);
            if(tag.theTag == "Coin") {
                // cout << "destroy: ----->" << entity << endl;
                DestroyEntityOnTileMap(entity);
            }
        }
        if(coordinator.HashComponent<PhysicsBody>(entity)) {
            // cout << "HashComponent<PhysicsBody>(entity)-------->"<< endl;
            // coordinator.m_PhysicsManagerSystem->RemoveB2Body(entity);
        }
        // coordinator.getEntityManager().DestroyEntity(entity);
        lua_pushboolean(L, !CheckEntityExist(entity));
        return 1;
    } else {
        lua_pushboolean(L, false);
        return 0;
    }
}



int Binding::lua_GetEntity(lua_State* L) {
    // cout << "calling: [C++] lua_GetEntity(...)" << endl;
    lua_pushinteger(L, static_cast<lua_Integer>(Binding::curEntity));
    return 1;
}

int Binding::lua_GetEntityByName(lua_State* L) {
    // cout << "calling: [C++] lua_GetEntityByName(...)" << endl;
    if(entities == NULL || entities->size() == 0) {
        return 0;
    }
    const char *requestName = luaL_checkstring(L, 1);
    for(auto& entity : *entities) {
        if(coordinator.HashComponent<Name>(entity)) {
            auto& name = coordinator.GetComponent<Name>(entity);
            if(strcmp(name.theName.c_str(), requestName) == 0) {
                lua_pushinteger(L, static_cast<lua_Integer>(entity));
                return 1;
            }
        }
    }
    return 0;
}

int Binding::Vec2DIndex(lua_State* L) {
    // cout << "calling: [C++] Vec2DIndex(...)" << endl;
    Vec2D* vec = (Vec2D*)lua_touserdata(L, 1);
    const char *key = luaL_checkstring(L, 2);

    if (strcmp(key, "x") == 0) {
        lua_pushinteger(L, vec->x);
        return 1;
    } else if (strcmp(key, "y") == 0) {
        lua_pushinteger(L, vec->y);
        return 1;
    }

    return 0;
}

int Binding::Vec2DNewIndex(lua_State* L) {
    // cout << "calling: [C++] Vec2DAssign(...)" << endl;
    Vec2D* vec = (Vec2D*)lua_touserdata(L, 1);
    const char *key = luaL_checkstring(L, 2);
    float value = (float)luaL_checknumber(L, 3);

    if (strcmp(key, "x") == 0) {
        vec->x = value;
        return 0;
    } else if (strcmp(key, "y") == 0) {
        vec->y = value;
        return 0;
    }

    return 0;
}

int Binding::Vec2DGC(lua_State* L) {
    // cout << "calling: [C++] Vec2DGC(...)" << endl;
    Vec2D* vec = (Vec2D*)lua_touserdata(L, 1);
    delete vec;
    return 0;
}

void Binding::RegisterVec2DComponentMetatable(lua_State* L) {
    luaL_newmetatable(L, "Vec2D");
    lua_pushcfunction(L, Vec2DGC);
    lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, Vec2DIndex);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, Vec2DNewIndex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L,1);
}

int Binding::lua_GetTransformComponent(lua_State* L) {
    // cout << "calling: [C++] lua_GetTransformComponent(...)" << endl;
    int paramNum = lua_gettop(L);
    Transform* transform;
    if(paramNum == 0) {
        transform = &(coordinator.GetComponent<Transform> (Binding::curEntity));
    } else {
        Entity requestEntity = static_cast<Entity>(lua_tointeger(L, 1));
        transform = &(coordinator.GetComponent<Transform> (requestEntity));
    }

    if(transform == nullptr) {
        return 0;
    }
    // cout << "[C++] transform.position.X:" << transform->position.x << endl;
    // cout << "[C++] transform.position.Y:" << transform->position.y << endl;
    lua_pushlightuserdata(L, transform);
    luaL_getmetatable(L, "Transform");
    lua_setmetatable(L, -2);
    return 1;
}

int Binding::TransformIndex(lua_State *L) {
    // cout << "calling: [C++] TransformIndex(...)" << endl;
    Transform* transform = (Transform*)lua_topointer(L, 1);
    const char *key = luaL_checkstring(L, 2);

    if (std::strcmp(key, "position") == 0) {
        // cout << "[C++] position.X:" << transform->position.x << endl;
        lua_pushlightuserdata(L, &(transform->position));
        // register new meta table
        luaL_getmetatable(L, "Vec2D");
        lua_setmetatable(L, -2);
        return 1;
    }

    return 0;
}

int Binding::TransformGC(lua_State* L) {
    // cout << "calling: [C++] TransformGC(...)" << endl;
    Transform* transform = (Transform*)lua_touserdata(L, 1);
    delete transform;
    return 0;
}

void Binding::RegisterTransformComponentMetatable(lua_State* L) {
    luaL_newmetatable(L, "Transform");
    lua_pushcfunction(L, TransformGC);
    lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, TransformIndex);
    lua_setfield(L, -2, "__index");
    lua_pop(L,1);
}

int Binding::lua_SetNameComponent(lua_State* L) {
    //cout << "calling: [C++] lua_SetNameComponent(...)" << endl;
    Entity requestEntity;
    int paramNum = lua_gettop(L);
    if(paramNum == 1) {
        requestEntity = Binding::curEntity;
    } else if(paramNum == 2) {
        requestEntity = static_cast<Entity>(lua_tointeger(L, 1));
    } else {
        lua_pushboolean(L, false);
        return 1;
    }

    if(coordinator.HashComponent<Name>(requestEntity)) {
        coordinator.RemoveComponent<Name>(requestEntity);
    }

    const char* name = lua_tostring(L, 2);
    coordinator.AddComponent(requestEntity, Name{name});
    if(coordinator.HashComponent<Name>(requestEntity)) {
        lua_pushboolean(L, strcmp(coordinator.GetComponent<Name>(requestEntity).theName.c_str(), name) == 0);
    } else {
        lua_pushboolean(L, false);
    }
    return 1;
}

int Binding::lua_RemoveNameComponent(lua_State* L) {
    //cout << "calling: [C++] lua_RemoveNameComponent(...)" << endl;
    Entity requestEntity;
    int paramNum = lua_gettop(L);
    if(paramNum == 0) {
        requestEntity = Binding::curEntity;
    } else if(paramNum == 1) {
        requestEntity = static_cast<Entity>(lua_tointeger(L, 1));
    } else {
        lua_pushboolean(L, false);
        return 1;
    }

    if(!coordinator.HashComponent<Name>(requestEntity)) {
        lua_pushboolean(L, true);
        return 1;
    }

    coordinator.RemoveComponent<Name>(requestEntity);
    lua_pushboolean(L, !coordinator.HashComponent<Name>(requestEntity));
    return 1;
}

int Binding::lua_GetTagComponent(lua_State* L) {
    //cout << "calling: [C++] lua_GetTagComponent(...)" << endl;
    Entity requestEntity;
    int paramNum = lua_gettop(L);
    if(paramNum == 0) {
        requestEntity = Binding::curEntity;
    } else if(paramNum == 1) {
        requestEntity = static_cast<Entity>(lua_tointeger(L, 1));
    } else {
        return 0;
    }

    Tag* tag;
    if(!coordinator.HashComponent<Tag>(requestEntity)) {
        cout << "No tag binded on this entity." << endl;
        return 0;
    }

    tag = &(coordinator.GetComponent<Tag> (requestEntity));

    if(tag == NULL) {
        cout << "Error: cannot get corresponding tag component" << endl;
        return 0;
    }
    
    lua_pushstring(L, (tag->theTag).c_str());
    return 1;
}

int Binding::lua_SetTagComponent(lua_State* L) {
    // cout << "calling: [C++] lua_SetTagComponent(...)" << endl;
    Entity requestEntity;
    int paramNum = lua_gettop(L);
    if(paramNum == 1) {
        requestEntity = Binding::curEntity;
    } else if(paramNum == 2) {
        requestEntity = static_cast<Entity>(lua_tointeger(L, 1));
    } else {
        return 0;
    }

    if(coordinator.HashComponent<Tag>(requestEntity)) {
        coordinator.RemoveComponent<Tag>(requestEntity);
    }

    const char* tag = lua_tostring(L, 2);
    coordinator.AddComponent(requestEntity, Tag{tag});
    if(coordinator.HashComponent<Tag>(requestEntity)) {
        lua_pushboolean(L, strcmp(coordinator.GetComponent<Tag>(requestEntity).theTag.c_str(), tag) == 0);
    } else {
        lua_pushboolean(L, false);
    }
    return 1;
}

int Binding::lua_RemoveTagComponent(lua_State* L) {
    // cout << "calling: [C++] lua_RemoveTagComponent(...)" << endl;
    Entity requestEntity;
    int paramNum = lua_gettop(L);
    if(paramNum == 0) {
        requestEntity = Binding::curEntity;
    } else if(paramNum == 1) {
        requestEntity = static_cast<Entity>(lua_tointeger(L, 1));
    } else {
        return 0;
    }

    if(!coordinator.HashComponent<Tag>(requestEntity)) {
        lua_pushboolean(L, true);
        return 1;
    }

    coordinator.RemoveComponent<Tag>(requestEntity);
    lua_pushboolean(L, !coordinator.HashComponent<Tag>(requestEntity));
    return 1;
}

// Return not the component but the sciprt path
int Binding::lua_GetScriptComponent(lua_State* L) {
    // cout << "calling: [C++] lua_GetScriptComponent(...)" << endl;
    Script* script;
    Entity requestEntity = static_cast<Entity>(lua_tointeger(L, 1));
    if(!coordinator.HashComponent<Script>(requestEntity)) {
        cout << "No script binded on this entity." << endl;
        return 0;
    }

    script = &(coordinator.GetComponent<Script> (requestEntity));

    if(script == nullptr) {
        cout << "Error: cannot get corresponding script component" << endl;
        return 0;
    }
    
    lua_pushstring(L, (script->scriptPath).c_str());
    return 1;
}

// Physics
// currently just return void
int Binding::lua_AddB2Body(lua_State* L) {
    // cout << "calling: [C++] lua_AddB2Body(...)" << endl;
    int paramNum = lua_gettop(L);
    Entity entity;
    if(paramNum == 1) {
        entity = Binding::curEntity;
    } else if (paramNum == 2) {
        entity = static_cast<Entity>(lua_tointeger(L, 1));
    } else {
        return 0;
    }
    
    const char* type = lua_tostring(L, -1);
    if(strcmp(type, "dynamic_solid") == 0) {
        b2Body* b2body = coordinator.m_PhysicsManagerSystem->CreateB2Body(entity, BodyType::DYNAMIC_SOLID);
        if(coordinator.HashComponent<PhysicsBody>(entity)) {
            coordinator.GetComponent<PhysicsBody>(entity).mBody = b2body;
        } else {
            coordinator.AddComponent<PhysicsBody>(entity, PhysicsBody{b2body});
        }
    }
    return 0;
}

int Binding::lua_RemoveB2Body(lua_State* L) {
    cout << "calling: [C++] lua_RemoveB2Body(...)" << endl;
    int paramNum = lua_gettop(L);
    Entity entity;
    if(paramNum == 0) {
        entity = Binding::curEntity;
    } else if (paramNum == 1) {
        entity = static_cast<Entity>(lua_tointeger(L, 1));
    } else {
        return 0;
    }
    coordinator.m_PhysicsManagerSystem->RemoveB2Body(entity);
    return 0;
}

int Binding::lua_SetVelocity(lua_State* L) {
    // cout << "calling: [C++] lua_SetVelocity(...)" << endl;
    int paramNum = lua_gettop(L);
    Entity entity;
    if(paramNum == 2) {
        entity = Binding::curEntity;
    } else if (paramNum == 3) {
        entity = static_cast<Entity>(lua_tointeger(L, 1));
    } else {
        return 0;
    }
    coordinator.m_PhysicsManagerSystem->SetB2BodyVx(entity, (float)lua_tonumber(L, -2));
    coordinator.m_PhysicsManagerSystem->SetB2BodyVy(entity, (float)lua_tonumber(L, -1));
    return 0;
}

int Binding::lua_AddForce(lua_State* L) {
    // cout << "calling: [C++] lua_AddForce(...)" << endl;
    int paramNum = lua_gettop(L);
    Entity entity;
    if(paramNum == 2) {
        entity = Binding::curEntity;
    } else if (paramNum == 3) {
        entity = static_cast<Entity>(lua_tointeger(L, 1));
    } else {
        return 0;
    }
    coordinator.m_PhysicsManagerSystem->AddB2BodyF(entity, (float)lua_tonumber(L, -2), (float)lua_tonumber(L, -1));
    return 0;
}

int Binding::lua_CheckCollision(lua_State* L) {
    // cout << "calling: [C++] lua_CheckCollision(...)" << endl;
    int paramNum = lua_gettop(L);
    if(paramNum == 2) {
        Entity entity1 = static_cast<Entity>(lua_tointeger(L, 1));
        Entity entity2 = static_cast<Entity>(lua_tointeger(L, 2));
        vector<pair<Entity, Entity>>* collisionSet = coordinator.m_PhysicsManagerSystem->GetContactPairs();
        bool result = find(collisionSet->begin(), collisionSet->end(), make_pair(entity1, entity2)) != collisionSet->end()
                    || find(collisionSet->begin(), collisionSet->end(), make_pair(entity2, entity1)) != collisionSet->end();
        lua_pushboolean(L, result);
        return 1;
    } else {
        return 0;
    }
}

int Binding::lua_OnCollision(lua_State* L) {
    // cout << "calling: [C++] lua_OnCollision(...)" << endl;
    int paramNum = lua_gettop(L);
    if(paramNum == 1) {
        Entity entity = static_cast<Entity>(lua_tointeger(L, 1));
        vector<pair<Entity, Entity>>* collisionSet = coordinator.m_PhysicsManagerSystem->GetContactPairs();
        set<Entity> collidingSet; 
        for(auto entityPair : *collisionSet) {
            if(entityPair.first == entity) {
                collidingSet.insert(entityPair.second);
            } else if (entityPair.second == entity) {
                collidingSet.insert(entityPair.first);
            }
        }
        lua_newtable(L);
        int tableIndex = lua_gettop(L);
        int i=0;
        for (auto it = collidingSet.begin(); it != collidingSet.end(); ++it) {
            lua_pushinteger(L, static_cast<lua_Integer>(*it));
            lua_rawseti(L, tableIndex, i+1);
            ++i;
        }
        return 1;
    } else {
        return 0;
    }
}

// Get Input
int Binding::lua_GetHorizontalInput(lua_State* L) {
    // cout << "calling: [C++] lua_GetHorizontalInput(...)" << endl;
    lua_pushinteger(L, Controlling::horizontalInput);
    return 1;
}


int Binding::lua_GetVerticalInput(lua_State*) {
    // cout << "calling: [C++] lua_GetVerticalInput(...)" << endl;
    lua_pushinteger(L, Controlling::verticalInput);
    return 1;
}

int Binding::lua_GetSpaceInput(lua_State*) {
    // cout << "calling: [C++] lua_GetSpaceInput(...)" << endl;
    lua_pushinteger(L, Controlling::spaceInput);
    return 1;
}

int Binding::lua_Win(lua_State*) {
    coordinator.m_EndGame = true;
    return 0;
}