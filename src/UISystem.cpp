#include "ECSCoordinator.hpp"
#include "UISystem.hpp"

extern ECSCoordinator coordinator;

void UISystem::Init()
{
    // OnMouseClicked_Left_Listener = [this](const Event& event){
    //     OnMouseClicked_Left(event);
    // };

    // coordinator.AddEventListener(EVENT::EVENT_ON_MOUSE_CLICKED_LEFT, OnMouseClicked_Left_Listener);
    coordinator.AddEventListener(EVENT::EVENT_ON_MOUSE_CLICKED_LEFT, [this](const Event& event){
        OnMouseClicked_Left(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_MOUSE_CLICKED_RIGHT, [this](const Event& event){
        OnMouseClicked_Right(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_KEYBOARD_CLICKED_O, [this](const Event& event){
        OnKeybardClicked_O(event);
    });
}

void UISystem::Update()
{
    for(auto& entity : m_Entities){
        
    }
}

void UISystem::Clear()
{
}

UISystem::~UISystem()
{
    // coordinator.RemoveEventListener(EVENT::EVENT_ON_MOUSE_CLICKED_LEFT, [this](const Event& event){
    //     OnMouseClicked_Left(event);
    // });

    // coordinator.RemoveEventListener(EVENT::EVENT_ON_MOUSE_CLICKED_RIGHT, [this](const Event& event){
    //     OnMouseClicked_Right(event);
    // });

    // coordinator.RemoveEventListener(EVENT::EVENT_ON_KEYBOARD_CLICKED_O, [this](const Event& event){
    //     OnKeybardClicked_O(event);
    // });
}

void UISystem::OnMouseClicked_Left(const Event &event)
{
    m_MouseX = event.GetParameter<int>("mouseX");
    m_MouseY = event.GetParameter<int>("mouseY");  
}

void UISystem::OnMouseClicked_Right(const Event &event)
{
    m_MouseX = event.GetParameter<int>("mouseX");
    m_MouseY = event.GetParameter<int>("mouseY");  
}

void UISystem::OnKeybardClicked_O(const Event &event)
{
    if(coordinator.m_State == GAME_STATE::MENU){
        Debug::PrintMessage("Click keyboard OOOOOOOO");
        coordinator.getSceneManager().EnterState(GAME_STATE::EDITOR);
    }
}
