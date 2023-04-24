#include "ECSCoordinator.hpp"
#include "InputSystem.hpp"
// #include "InputSystem.hpp"

extern ECSCoordinator coordinator;

void InputSystem::Init()
{
    Debug::PrintMessage("Init input system");
}

void InputSystem::Update()
{
    // Keyboad event
    const Uint8 *keyboard_State = SDL_GetKeyboardState(NULL);

    // mouse event
    Uint32 mouseState = SDL_GetMouseState(&m_MouseX, &m_MouseY);

    // keyboard detection
    if (keyboard_State[SDL_SCANCODE_W]){
        this->DispatchKeyboardEvent_W();
    }
    if (keyboard_State[SDL_SCANCODE_A]){
        this->DispatchKeyboardEvent_A();
    }
    if (keyboard_State[SDL_SCANCODE_S]){
        this->DispatchKeyboardEvent_S();
    }
    if (keyboard_State[SDL_SCANCODE_D]){
        this->DispatchKeyboardEvent_D();
    }
    if (keyboard_State[SDL_SCANCODE_O]){
        this->DispatchKeyboardEvent_O();
    }
    if (keyboard_State[SDL_SCANCODE_I]){
        this->DispatchKeyboardEvent_I();
    }
    if (keyboard_State[SDL_SCANCODE_U]){
        this->DispatchKeyboardEvent_U();
    }
    if (keyboard_State[SDL_SCANCODE_SPACE]){
        this->DispatchKeyboardEvent_SPACE();
    }

    // mouse detection
    // 使用事件中心传递鼠标的坐标给tilemap子系统，达到解耦目的
    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        // cout << m_MouseX << ", " << m_MouseY << endl;
        // AudioManager::GetInstance().PlaySound(AUDIO_COIN, 60, 0);
        Debug::PrintMessage("left mouse clicked");
        this->DispatchMousePosition_LeftClick(m_MouseX, m_MouseY);
    }
    else if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        // cout << m_MouseX << ", " << m_MouseY << endl;
        Debug::PrintMessage("right mouse clicked");
        this->DispatchMouseRightClickEvent_RightClick(m_MouseX, m_MouseY);
    }

    for (auto &entity : m_Entities)
    {
        switch (coordinator.m_State)
        {
        case GAME_STATE::EDITOR:
            HandleEditorState(entity, mouseState, keyboard_State);
            break;
        case GAME_STATE::GAME:
            HandleGameState(entity, mouseState, keyboard_State);
            break;
        default:
            break;
        }
    }

    // SDL_Event event;
    // // 假设在editor中可以按下鼠标一直检测按下
    // while (SDL_PollEvent(&event))
    // {

    //     switch (event.type)
    //     {
    //     case SDL_MOUSEBUTTONDOWN:
    //         if (event.button.button == SDL_BUTTON_LEFT)
    //         {
    //             AudioManager::GetInstance().PlaySound(AUDIO_COIN, 60);
    //         }
    //         break;
    //     case SDL_MOUSEBUTTONUP:
    //         if (event.button.button == SDL_BUTTON_RIGHT)
    //         {
    //         }
    //         break;
    //     case SDL_QUIT:
            
    //     default:
    //         break;
    //     }
    // }
    // if (coordinator.m_State == GAME_STATE::GAME || coordinator.m_State == GAME_STATE::MENU)
    // {
        
    // }
}

void InputSystem::Clear()
{
}

void InputSystem::DispatchMousePosition_LeftClick(int mouseX, int mouseY)
{
    Event event(EVENT::EVENT_ON_MOUSE_CLICKED_LEFT);
    event.SetParameter<int>("mouseX", mouseX);
    event.SetParameter<int>("mouseY", mouseY);

    coordinator.TriggerEvent(event);
}

void InputSystem::DispatchMouseRightClickEvent_RightClick(int mouseX, int mouseY)
{
    Event event(EVENT::EVENT_ON_MOUSE_CLICKED_RIGHT);
    event.SetParameter<int>("mouseX", mouseX);
    event.SetParameter<int>("mouseY", mouseY);

    coordinator.TriggerEvent(event);
}

void InputSystem::DispatchKeyboardEvent_W()
{
    Event event(EVENT::EVENT_ON_KEYBOARD_CLICKED_W);
    coordinator.TriggerEvent(event);
}

void InputSystem::DispatchKeyboardEvent_A()
{
    Event event(EVENT::EVENT_ON_KEYBOARD_CLICKED_A);
    coordinator.TriggerEvent(event);
}

void InputSystem::DispatchKeyboardEvent_S()
{
    Event event(EVENT::EVENT_ON_KEYBOARD_CLICKED_S);
    coordinator.TriggerEvent(event);
}

void InputSystem::DispatchKeyboardEvent_D()
{
    Event event(EVENT::EVENT_ON_KEYBOARD_CLICKED_D);
    coordinator.TriggerEvent(event);
}

void InputSystem::DispatchKeyboardEvent_O()
{
    Event event(EVENT::EVENT_ON_KEYBOARD_CLICKED_O);
    coordinator.TriggerEvent(event);
}

void InputSystem::DispatchKeyboardEvent_I()
{
    Event event(EVENT::EVENT_ON_KEYBOARD_CLICKED_I);
    coordinator.TriggerEvent(event);
}

void InputSystem::DispatchKeyboardEvent_U()
{
    Event event(EVENT::EVENT_ON_KEYBOARD_CLICKED_U);
    coordinator.TriggerEvent(event);
}

void InputSystem::DispatchKeyboardEvent_SPACE()
{
    Event event(EVENT::EVENT_ON_KEYBOARD_CLICKED_SPACE);
    coordinator.TriggerEvent(event);
}

void InputSystem::HandleEditorState(const Entity &entity, Uint32 mouseState, const Uint8 *keyboard_State)
{

    if (coordinator.HashComponent<Enemy>(entity))
    {
        // TODO: 拖动对应的prefab到游戏中
        // 然后要执行对应的存储方法（一次性存储，多次存储消耗资源）
    }

    if (coordinator.HashComponent<Tilemap>(entity))
    {
        // mouse detection
        // SDL_BUTTON(SDL_BUTTON_LEFT) returns a bitmask, not a bool, so we use & to check if it fits our goal
        auto& tilemap = coordinator.GetComponent<Tilemap>(entity);
    }
}

void InputSystem::HandleGameState(const Entity &entity, Uint32 mouseState, const Uint8 *keyboard_State)
{
    // player movement
    // if (coordinator.HashComponent<InPut>(entity))
    // {
    //     if (coordinator.HashComponent<Player>(entity))
    //     {
    //         auto &player = coordinator.GetComponent<Player>(entity);
    //         auto &transform = coordinator.GetComponent<Transform>(entity);
    //         auto &sprite = coordinator.GetComponent<Sprite>(entity);
    //         int playerSpeed = player.speed;

    //         if (keyboard_State[SDL_SCANCODE_A])
    //         {
    //             transform.position.x -= playerSpeed;
    //             sprite.m_Dest.x -= playerSpeed;
    //         }
    //         if (keyboard_State[SDL_SCANCODE_D])
    //         {
    //             transform.position.x += playerSpeed;
    //             sprite.m_Dest.x += playerSpeed;
    //         }
    //         if (keyboard_State[SDL_SCANCODE_W])
    //         {
    //             transform.position.y += playerSpeed;
    //             sprite.m_Dest.y -= playerSpeed;
    //         }
    //         if (keyboard_State[SDL_SCANCODE_S])
    //         {
    //             transform.position.y -= playerSpeed;
    //             sprite.m_Dest.y += playerSpeed;
    //         }

    //         if (keyboard_State[SDL_SCANCODE_R])
    //         {
    //         }
    //     }
    // }
}