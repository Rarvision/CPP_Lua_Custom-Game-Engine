#include <assert.h>

#include "ECSCoordinator.hpp"
#include "Engine.hpp"
#include "PlayerControllerSystem.hpp"

extern ECSCoordinator coordinator;

void PlayerControllerSystem::Init(){

    coordinator.AddEventListener(EVENT::EVENT_ON_KEYBOARD_CLICKED_A, [this](const Event& event){
        OnKeyboardClicked_A(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_KEYBOARD_CLICKED_D, [this](const Event& event){
        OnKeyboardClicked_D(event);
    });
}

void PlayerControllerSystem::Update(){

    for(auto& entity : m_Entities){
        // if (coordinator.HashComponent<Player>(entity)){
        //     if (coordinator.HashComponent<InPut>(entity)){
        //         auto &sprite = coordinator.GetComponent<Sprite>(entity);

        //     }
        // }
        // cout << "PlayerControllerSystem::MovePlayer" << m_Entities.size() << endl;
    }
}

void PlayerControllerSystem::Clear()
{

}

Player PlayerControllerSystem::CreatePlayer()
{
    return Player();
}

void PlayerControllerSystem::OnKeyboardClicked_A(const Event &event)
{
    // cout << "PlayerControllerSystem::OnKeyboardClicked_A clicked" << endl;
    MovePlayer(-1, 50);
}

void PlayerControllerSystem::OnKeyboardClicked_D(const Event &event)
{
    MovePlayer(1, 50);
}

void PlayerControllerSystem::MovePlayer(int direction, int boundaryOffSet)
{
    if (coordinator.m_State == GAME_STATE::GAME)
    {
        // cout << "PlayerControllerSystem::MovePlayer" << m_Entities.size() << endl;
        for (auto &entity : m_Entities)
        {
            if (coordinator.HashComponent<Player>(entity))
            {
                if (coordinator.HashComponent<InPut>(entity))
                {
                    auto &sprite = coordinator.GetComponent<Sprite>(entity);
                    auto &player = coordinator.GetComponent<Player>(entity);
                    // sprite.m_Dest.x += player.speed * direction;
                    sprite.m_Dest.x = Utils::GetInstance().clamp(sprite.m_Dest.x, 10, coordinator.m_Tilemap.m_MaxMapWidth - boundaryOffSet - EDITOR_WIDTH);
                    DispatchPlayerMovePosition(sprite.m_Dest.x, sprite.m_Dest.y);
                    // cout << "coordinator.m_Tilemap.m_MaxMapWidth" << coordinator.m_Tilemap.m_MaxMapWidth << endl;
                    // cout << "sprite.m_Dest.x " << sprite.m_Dest.x << endl;
                }
            }
        }
    }
}

void PlayerControllerSystem::DispatchPlayerMovePosition(int x, int y)
{
    Event event(EVENT::EVENT_ON_PLAYER_MOVE);
    event.SetParameter<int>("playerX", x);
    event.SetParameter<int>("playerY", y);
    
    coordinator.TriggerEvent(event);
}
