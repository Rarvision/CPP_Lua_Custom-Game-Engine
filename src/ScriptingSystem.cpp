#include "ECSCoordinator.hpp"
#include "Binding.hpp"

extern ECSCoordinator coordinator;

void ScriptingSystem::Init() 
{
    // std::cout << "ScriptingSystem::Init() called" << std::endl;

    if(Binding::L == NULL) 
    {
        // std::cout << "ScriptingSystem::Init() entered Binding::L == NULL" << std::endl;
        Binding::L = luaL_newstate();
        if(Binding::L == NULL) {
            // cout << "Unable to initialize the scripting system." << endl;
            return;
        }
        luaL_openlibs(Binding::L);

        BindFunctions();

        // give access of entity list to Bind class
        Binding::entities = &m_Entities; 
    }

}

void ScriptingSystem::BindFunctions() {
    luaL_Reg regFunctions[] = {
        {"create_entity", Binding::lua_CreateEntity},
        {"destroy_entity", Binding::lua_DestroyEntity},

        {"get_entity", Binding::lua_GetEntity},
        {"get_entity_by_name", Binding::lua_GetEntityByName},

        {"get_transform_component", Binding::lua_GetTransformComponent},
            
        {"set_name_component", Binding::lua_SetNameComponent},
        {"remove_name_component", Binding::lua_RemoveNameComponent},
            
        {"get_tag_component", Binding::lua_GetTagComponent},
        {"set_tag_component", Binding::lua_SetTagComponent},
        {"remove_tag_component", Binding::lua_RemoveTagComponent},
            
        {"get_script_component", Binding::lua_GetScriptComponent},
            
        {"add_b2_body", Binding::lua_AddB2Body},
        {"remove_b2_body", Binding::lua_RemoveB2Body},
        {"set_velocity", Binding::lua_SetVelocity},
        {"add_force", Binding::lua_AddForce},
        {"check_collision", Binding::lua_CheckCollision},
        {"on_collision", Binding::lua_OnCollision},

        {"get_horizontal_input", Binding::lua_GetHorizontalInput},
        {"get_vertical_input", Binding::lua_GetVerticalInput},
        {"get_space_input", Binding::lua_GetSpaceInput},

        {"win", Binding::lua_Win},
        
        { NULL, NULL } // End mark
    };

    luaL_newlib(Binding::L, regFunctions);
    lua_setglobal(Binding::L, "engine");
        
    // register meta tables for all possible component
    Binding::RegisterVec2DComponentMetatable(Binding::L);
    Binding::RegisterTransformComponentMetatable(Binding::L);
}

void ScriptingSystem::Clear() {
    std::cout << "ScriptingSystem::Clear() called" << std::endl;
    init_set.clear();
    lua_settop(Binding::L,0);
    luaL_openlibs(Binding::L);
    BindFunctions();

}

void ScriptingSystem::Update() {
    // std::cout << "ScriptingSystem::Update() called" << std::endl;
    for(auto& entity : m_Entities) {
        if(coordinator.HashComponent<Script>(entity)) {
            auto& script = coordinator.GetComponent<Script>(entity);
            Binding::curEntity = entity;
            if(init_set.find(entity) != init_set.end()) {
                Execute(script.scriptPath);
            } else {
                init_set.insert(entity);
                Load(script.scriptPath);
            }
        }
    }
}

void ScriptingSystem::Load(string s) {
    // std::cout << "ScriptingSystem::Load()" << std::endl;
    // std::cout << "s.scriptPath: " <<  s << ".lua" << std::endl;
    string path = "./scripts/" + s + ".lua";
    if(Binding::CheckLua(Binding::L, luaL_loadfile(Binding::L, path.c_str()))) {
        lua_pcall(Binding::L, 0, 1, 0);
        lua_setglobal(Binding::L, s.c_str());
        lua_getglobal(Binding::L, s.c_str());
        lua_getfield(Binding::L, -1, "start");
        if (lua_isfunction(Binding::L, -1)) {
            lua_pcall(Binding::L, 0, 0, 0);
        }
    }

}

void ScriptingSystem::Execute(string s) {
    // std::cout << "ScriptingSystem::Execute()" << std::endl;
    // std::cout << "s.scriptPath: " <<  s << ".lua" << std::endl;
    lua_getglobal(Binding::L, s.c_str());
    lua_getfield(Binding::L, -1, "update");
    if (lua_isfunction(Binding::L, -1)) {
        lua_pcall(Binding::L, 0, 0, 0);
    }
}


