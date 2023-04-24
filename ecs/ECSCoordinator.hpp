#pragma once

// managers
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "EventManager.hpp"
#include "SceneManager.hpp"
#include "AudioManager.hpp"

// subsystems
#include "PlayerControllerSystem.hpp"
#include "SpriteSystem.hpp"
#include "TilemapSystem.hpp"
#include "RenderSystem.hpp"
#include "SDLGraphicsSystem.hpp"
#include "InputSystem.hpp"
#include "LevelEditorSystem.hpp"
#include "UISystem.hpp"
#include "CameraSystem.hpp"
#include "PhysicsManagerSystem.hpp"
#include "GameplaySystem.hpp"
#include "ScriptingSystem.hpp"
#include "ControllerSystem.hpp"

// components
#include "InPut.hpp"
#include "Player.hpp"
#include "Transform.hpp"
#include "Sprite.hpp"
#include "Tilemap.hpp"
#include "Renderer.hpp"
#include "Enemy.hpp"
#include "Editor.hpp"
#include "Camera.hpp"
#include "UIComponent.hpp"
#include "Script.hpp"
#include "Name.hpp"
#include "Tag.hpp"
#include "Controlling.hpp"
#include "PhysicsBody.hpp"
#include "Tile.hpp"

#include "FileReader.hpp"
#include "Utils.hpp"

class ECSCoordinator{
public:
    ECSCoordinator();

    void Init();

    void Update();

    void Render();

    // --- Component Manager ---

    /// @brief register component function, you must register component first to use the component
    /// @tparam T component name
    template<typename T>
    void RegisterComponent();

    /// @brief get the component unique id
    /// @tparam T component type
    /// @return component id
    template<typename T>
    ComponentType GetComponentTypeUniqueIndex();

    /// @brief add a component to the entity
    /// @tparam T component type
    /// @param entity 
    /// @param component 
    template<typename T>
    void AddComponent(Entity entity, T component);

    /// @brief remove a compnent from the entity
    /// @tparam T component type
    /// @param entity 
    template<typename T>
    void RemoveComponent(Entity entity);

    /// @brief get the component from the entity
    /// @tparam T component type
    /// @param entity 
    /// @return 
    template<typename T>
    T& GetComponent(Entity entity);

    // --- Entity Manager ---
    Entity CreateEntity()
	{
		return m_EntityManager->CreateEntity();
	}

    void DestroyEntity(Entity);

    // TODO: 加destroy entity的方法，直接调ECS中的函数
    // void DestroyEntity(Entity entity);

    // --- System Manager ---

    /// @brief register a system (a system cannot be registered twice)
    /// @tparam T system type
    /// @return system shared_ptr
    template<typename T>
    shared_ptr<T> RegisterSystem();

    /// @brief set a signature for a system
    /// @tparam T system type
    /// @param signature 
    template<typename T>
    void SetSignature(Signature signature);

    /// @brief check if the entity has the component
    /// @tparam T component type
    /// @param entity 
    /// @return 
    template<typename T>
    bool HashComponent(Entity entity);

    // --- Event Manager ---
    inline void AddEventListener(EVENT eventId, function<void(Event&)> const& callBack);

    inline void RemoveEventListener(EVENT eventId, function<void(Event&)> const& callBack);

    inline void TriggerEvent(Event& event);

    inline void TriggerEvent(EVENT eventId);

    SceneManager &getSceneManager() { return *m_SceneManager; }
    ComponentManager &getComponentManager() { return *m_ComponentManager; }
    EntityManager &getEntityManager() { return *m_EntityManager; }
    SystemManager &getSystemManager() { return *m_SystemManager; }
    EventManager &getEventManager() { return *m_EventManager; }

    shared_ptr<GameplaySystem> getGameplaySystem() const {return m_GameplaySystem;}

    // TODO: 将所有的这些都设置为私有，并提供get方法获取
    shared_ptr<RenderSystem>                    m_RenderSystem;
    // shared_ptr<PlayerControllerSystem>          m_PlayerControllerSystem;
    shared_ptr<SpriteSystem>                    m_SpriteSystem;
    shared_ptr<TilemapSystem>                   m_TilemapSystem;
    shared_ptr<SDLGraphicsSystem>               m_SDLGraphicsSystem;
    shared_ptr<InputSystem>                     m_InputSystem;
    shared_ptr<LevelEditorSystem>               m_LevelEditorSystem;
    shared_ptr<UISystem>                        m_UISystem;
    shared_ptr<CameraSystem>                    m_CameraSystem;
    shared_ptr<ScriptingSystem>                 m_ScriptingSystem;
    shared_ptr<ControllerSystem>                m_ControllerSystem;
    
    shared_ptr<PhysicsManagerSystem>            m_PhysicsManagerSystem;

    /// @brief the state that controls the state
    GAME_STATE                                  m_State;

    /// @brief save the tilemap object here to share data
    Tilemap                                     m_Tilemap;

    /// @brief save the tilemap object before the map data is changed
    Tilemap                                     m_TempTilemap;

    Entity                                      m_PlayerEntity;

    bool                                        m_EndGame = false;
    bool                                        m_SaveGame = false;
private:
    shared_ptr<GameplaySystem>                  m_GameplaySystem;
    
    unique_ptr<SceneManager>                    m_SceneManager;
    unique_ptr<ComponentManager>                m_ComponentManager;
    unique_ptr<EntityManager>                   m_EntityManager;
    unique_ptr<SystemManager>                   m_SystemManager;
    unique_ptr<EventManager>                    m_EventManager;
};


template<typename T>
void ECSCoordinator::RegisterComponent(){
    m_ComponentManager->RegisterComponent<T>();
}

template<typename T>
ComponentType ECSCoordinator::GetComponentTypeUniqueIndex(){
    return m_ComponentManager->GetComponentTypeUniqueIndex<T>();
}

template<typename T>
void ECSCoordinator::AddComponent(Entity entity, T component){
    // bind the componet with entity
    m_ComponentManager->AddComponent<T>(entity, component);

    // get signature via entity id
    auto signature = m_EntityManager->GetSignatureByEntity(entity);

    // set signature in bitset to show which component is TRUE now
    signature.set(this->GetComponentTypeUniqueIndex<T>(), true);

    m_EntityManager->SetSignature(entity, signature);

    m_SystemManager->EntitySignatureChanged(entity, signature);

}

template<typename T>
void ECSCoordinator::RemoveComponent(Entity entity){
    m_ComponentManager->RemoveComponent<T>(entity);

    auto signature = m_EntityManager->GetSignatureByEntity(entity);
    signature.set(this->GetComponentTypeUniqueIndex<T>(), false);
    m_EntityManager->SetSignature(entity, signature);

    m_SystemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
T& ECSCoordinator::GetComponent(Entity entity){
    return m_ComponentManager->GetComponent<T>(entity);
}

template<typename T>
shared_ptr<T> ECSCoordinator::RegisterSystem(){
    return m_SystemManager->RegisterSystem<T>();
}

template<typename T>
void ECSCoordinator::SetSignature(Signature signature){
    m_SystemManager->SetSignature<T>(signature);
}

template<typename T>
bool ECSCoordinator::HashComponent(Entity entity){
    return m_ComponentManager->HasComponent<T>(entity);
}

// ////////  --- event system ---  ///////////
inline void ECSCoordinator::AddEventListener(EVENT eventId, function<void(Event&)> const& callBack){
    m_EventManager->AddListener(eventId, callBack);
}

inline void ECSCoordinator::RemoveEventListener(EVENT eventId, function<void(Event&)> const& callBack){
    m_EventManager->RemoveListener(eventId, callBack);
}

inline void ECSCoordinator::TriggerEvent(Event& event){
    m_EventManager->Trigger(event);
}

inline void ECSCoordinator::TriggerEvent(EVENT eventId){
    m_EventManager->Trigger(eventId);
}