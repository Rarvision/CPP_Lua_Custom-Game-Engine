#include "ECSCoordinator.hpp"

ECSCoordinator::ECSCoordinator(){
    m_ComponentManager  = std::make_unique<ComponentManager>();
    m_EntityManager     = std::make_unique<EntityManager>();
    m_SystemManager     = std::make_unique<SystemManager>();
    m_EventManager      = std::make_unique<EventManager>();
    m_SceneManager      = std::make_unique<SceneManager>();

    m_State             = GAME_STATE::MENU;
}

void ECSCoordinator::Init()
{
    m_SceneManager->InitMenu();
}

void ECSCoordinator::Update()
{
    // menu state required
    m_SpriteSystem->Update();
    m_InputSystem->Update();

    if (m_State == GAME_STATE::GAME)
    {

        if(m_CameraSystem != nullptr)
        {
            m_CameraSystem->Update();
        }

        if (m_PhysicsManagerSystem != nullptr)
        {
            m_PhysicsManagerSystem->Update();
        }

        if (m_ScriptingSystem != nullptr)
        {
            m_ScriptingSystem->Update();
        }

        if (m_ControllerSystem != nullptr)
        {
            m_ControllerSystem->Update();
        }
    }

    if (m_State == GAME_STATE::EDITOR)
    {
        m_LevelEditorSystem->Update();
    }
}

void ECSCoordinator::Render()
{
    m_RenderSystem->Render();
}

void ECSCoordinator::DestroyEntity(Entity entity){
    
    m_EntityManager->DestroyEntity(entity);
    
    m_ComponentManager->DestroyEntity(entity);

    m_SystemManager->DestroyEntity(entity);
}
