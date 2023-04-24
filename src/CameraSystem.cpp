#include "ECSCoordinator.hpp"
#include "CameraSystem.hpp"

extern ECSCoordinator coordinator;

void CameraSystem::Init()
{
    this->m_HasPlayerEntity = false;
    
    coordinator.AddEventListener(EVENT::EVENT_ON_KEYBOARD_CLICKED_A, [this](const Event& event){
        OnKeyboardClicked_A(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_KEYBOARD_CLICKED_D, [this](const Event& event){
        OnKeyboardClicked_D(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_KEYBOARD_CLICKED_W, [this](const Event& event){
        OnKeyboardClicked_W(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_KEYBOARD_CLICKED_S, [this](const Event& event){
        OnKeyboardClicked_S(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_TILEMAP_CHANGED, [this](const Event& event){
        OnTilemapChanged(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_PLAYER_MOVE, [this](const Event& event){
        OnPlayerMovePosition(event);
    });
}

void CameraSystem::Update()
{
    for (auto& entity : m_Entities)
    {
        if(coordinator.m_State == GAME_STATE::GAME){
            if(this->m_HasPlayerEntity){
                Follow(m_PlayerMoveX, m_PlayerMoveY);
            }
            // if(coordinator.HashComponent<Player>(entity)){
            //     auto& playerSprite = coordinator.GetComponent<Sprite>(entity);
            //     Follow(playerSprite.m_Dest.x, playerSprite.m_Dest.y);
            // }
        }
    }
}

void CameraSystem::Clear()
{
}

void CameraSystem::Follow(int x, int y)
{
    MoveCamera(x, y);
}

void CameraSystem::MoveCamera(int x, int y)
{
    // 定义边界矩形
    // m_CameraInnerViewport.x = m_Camera.cameraX + (m_Camera.viewportWidth - m_CameraInnerViewport.w) / 2;
    // m_CameraInnerViewport.y = m_Camera.cameraY + (m_Camera.viewportHeight - m_CameraInnerViewport.h) / 2;
    // cout <<  m_CameraInnerViewport.x << ", " <<  m_CameraInnerViewport.y << endl;
    // // 检查玩家是否越过边界矩形
    // if (x < m_CameraInnerViewport.x)
    // {
    //     // 由于玩家是世界坐标，在越过边界时，和视口是有一个offset的，那么摄像机也要基于这个offset去移动
    //     m_Camera.cameraX -= (m_CameraInnerViewport.x - x);
    //     m_Camera.cameraX = Utils::GetInstance().clamp(m_Camera.cameraX, 0, m_Camera.cameraMaxX);
    // }
    // else if (x > m_CameraInnerViewport.x + m_CameraInnerViewport.w)
    // {
    //     // 这里无非就是多加了个视口的宽度，然后将x减去（此时说明x已经越过了整个视口），最后将camera加上这个offset
    //     m_Camera.cameraX += (x - (m_CameraInnerViewport.x + m_CameraInnerViewport.w));
    //     m_Camera.cameraX = Utils::GetInstance().clamp(m_Camera.cameraX, 0, m_Camera.cameraMaxX);
    // }

    // if (y < m_CameraInnerViewport.y)
    // {
    //     m_Camera.cameraY -= (m_CameraInnerViewport.y - y);
    // }
    // else if (y > m_CameraInnerViewport.y + m_CameraInnerViewport.h)
    // {
    //     m_Camera.cameraY += (y - (m_CameraInnerViewport.y + m_CameraInnerViewport.h));
    // }

    SDL_FRect innner = {
        m_Camera.cameraX + m_CameraInnerViewport.w * 0.5f * (1 - m_Camera.edge),
        m_Camera.cameraY + m_CameraInnerViewport.h * 0.5f * (1 - m_Camera.edge),
        m_Camera.cameraX + m_CameraInnerViewport.w * 0.5f * (1 + m_Camera.edge),
        m_Camera.cameraY + m_CameraInnerViewport.h * 0.5f * (1 + m_Camera.edge)};

    if (x < innner.x)
        m_Camera.cameraX -= (innner.x - x);
    else if (x > innner.w)
        m_Camera.cameraX += (x - innner.w);

    if (y < innner.y)
    {
        m_Camera.cameraY -= (innner.y - y);
    }
    else if (y > innner.h)
    {
        m_Camera.cameraY += (y - innner.h);
    }

    m_Camera.cameraX = Utils::GetInstance().clamp(m_Camera.cameraX, 0, m_Camera.cameraMaxX);

    DispatchCameraMovementPosition(m_Camera.cameraX, m_Camera.cameraY);
}

void CameraSystem::EnterState(const GAME_STATE &state)
{
    cout << "Entering CameraSystem" << endl;
    switch (state)
    {
    case GAME_STATE::EDITOR:
        m_Camera.cameraX = 0;
        m_Camera.cameraY = 0;
        m_Camera.viewportWidth = WINDOW_WIDTH - EDITOR_WIDTH;
        m_Camera.viewportHeight = 1080;
        m_Camera.worldWidth = coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_MapX * coordinator.m_Tilemap.m_ScaleFactor; // 这里需要知道tilemap的大小，要基于tilemap的属性去计算游戏地图的最终大小
        m_Camera.worldHeight = coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_MapY * coordinator.m_Tilemap.m_ScaleFactor;
        m_Camera.cameraMaxX = coordinator.m_Tilemap.m_MaxMapWidth - m_Camera.viewportWidth;

        m_CameraSpeed = coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_ScaleFactor;
        break;
    case GAME_STATE::GAME:
        m_Camera.cameraX = 0;
        m_Camera.cameraY = 0;
        m_Camera.viewportWidth = 1920;
        m_Camera.viewportHeight = 1080;
        m_Camera.worldWidth = coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_MapX * coordinator.m_Tilemap.m_ScaleFactor; // 这里需要知道tilemap的大小，要基于tilemap的属性去计算游戏地图的最终大小
        m_Camera.worldHeight = coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_MapY * coordinator.m_Tilemap.m_ScaleFactor;
        m_Camera.cameraMaxX = coordinator.m_Tilemap.m_MaxMapWidth - m_Camera.viewportWidth;
        // cout << "m_Camera.worldWidth in GAME is " << m_Camera.worldWidth << endl;
        // cout << &m_Camera << " in game enter state" << endl;

        // set camera inner viewport
        m_CameraInnerViewport.w = 600;
        m_CameraInnerViewport.h = 400;
        m_CameraInnerViewport.x = m_Camera.cameraX + (m_Camera.viewportWidth - m_CameraInnerViewport.w) / 2;
        m_CameraInnerViewport.y = m_Camera.cameraY + (m_Camera.viewportHeight - m_CameraInnerViewport.h) / 2;

        m_CameraSpeed = coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_ScaleFactor;
        if(coordinator.m_PlayerEntity != -1){
            m_HasPlayerEntity = true;
        }
        break;
    default:
        break;
    }
}

Camera CameraSystem::CreateCamera(float cameraX, float cameraY, float viewportWidth, float viewportHeight, float worldWidth, float worldHeight, float edge)
{
    Camera _camera;
    m_Camera = _camera;
    m_Camera.cameraX = cameraX;
    m_Camera.cameraY = cameraY;
    m_Camera.viewportWidth = viewportWidth;
    m_Camera.viewportHeight = viewportHeight;
    m_Camera.worldWidth = worldWidth;
    m_Camera.worldHeight = worldHeight;
    m_Camera.viewport = {cameraX, cameraY, cameraX + viewportWidth, cameraY + viewportHeight};
    m_Camera.edge = edge;
    return m_Camera;
}

void CameraSystem::OnKeyboardClicked_A(const Event &event)
{
    // Debug::PrintMessage("A is clicked in CameraSystem");
    if (coordinator.m_State == GAME_STATE::EDITOR)
    {
        m_Camera.cameraX -= m_CameraSpeed;
        // x 所能移动的最大距离
        // 能移动的最大距离 = 总地图长度 - 摄像机的宽度
        m_Camera.cameraX = Utils::GetInstance().clamp(m_Camera.cameraX, 0, m_Camera.cameraMaxX);
        DispatchCameraMovementPosition(m_Camera.cameraX, m_Camera.cameraY);
    }
}

void CameraSystem::OnKeyboardClicked_D(const Event &event)
{
    if (coordinator.m_State == GAME_STATE::EDITOR)
    {
        m_Camera.cameraX += m_CameraSpeed;

        // x 所能移动的最大距离
        m_Camera.cameraX = Utils::GetInstance().clamp(m_Camera.cameraX, 0, m_Camera.cameraMaxX);
        
        DispatchCameraMovementPosition(m_Camera.cameraX, m_Camera.cameraY);
    }
}

void CameraSystem::OnKeyboardClicked_W(const Event &event)
{
    // if(coordinator.coordinator.m_State == GAME_STATE::EDITOR){
    //     m_Camera.cameraY += m_CameraSpeed;

    //     // y 所能移动的最大距离 
    //     m_Camera.cameraY = clamp(m_Camera.cameraX, 0, 320);
    //     cout << "m_Camera.cameraX is " << m_Camera.cameraX << endl;

    //     DispatchCameraMovementPosition(m_Camera.cameraX, m_Camera.cameraY);
    // }
}

void CameraSystem::OnKeyboardClicked_S(const Event &event)
{

}

void CameraSystem::OnPlayerMovePosition(const Event &event)
{
    m_PlayerMoveX = event.GetParameter<int>("playerX");
    m_PlayerMoveY = event.GetParameter<int>("playerY");
}

void CameraSystem::OnTilemapChanged(const Event &event)
{
    m_Camera.cameraMaxX = coordinator.m_Tilemap.m_MaxMapWidth - m_Camera.viewportWidth;
    m_Camera.worldWidth = coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_MapX * coordinator.m_Tilemap.m_ScaleFactor; // 这里需要知道tilemap的大小，要基于tilemap的属性去计算游戏地图的最终大小
    m_Camera.worldHeight = coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_MapY * coordinator.m_Tilemap.m_ScaleFactor;
}

void CameraSystem::DispatchCameraMovementPosition(int x, int y)
{
    Event event(EVENT::EVENT_ON_CAMERA_MOVE);
    event.SetParameter<int>("cameraX", m_Camera.cameraX);
    event.SetParameter<int>("cameraSpeed", m_CameraSpeed);

    coordinator.TriggerEvent(event);
}