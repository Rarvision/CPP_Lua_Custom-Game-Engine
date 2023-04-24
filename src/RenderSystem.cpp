#if defined(LINUX) || defined(MINGW)
	#include <SDL2/SDL.h>
#else
	// Windows and Mac use a different path
	// If you have compilation errors, change this as needed.
	#include <SDL.h>
#endif
#include <algorithm>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include <imgui_sdl.h>

#include <SDL_image.h>
#include "ECSCoordinator.hpp"

extern ECSCoordinator coordinator;


void RenderSystem::Init(){
    m_Render = coordinator.m_SDLGraphicsSystem->GetSDLRenderer();
    m_Window = coordinator.m_SDLGraphicsSystem->GetWindow();
    // int tilePixel = 16;
    // int offsetX   = 5;

    // [editor mode] the start of the game position (render position)
    m_Camera.x = 0;
    m_Camera.y = 0;
    m_Camera.w = (WINDOW_WIDTH - EDITOR_WIDTH);
    m_Camera.h = WINDOW_HIEGHT;

    m_Editor_GameDest.x = EDITOR_WIDTH;
    m_Editor_GameDest.y = 0;
    m_Editor_GameDest.w = WINDOW_WIDTH - EDITOR_WIDTH; // Set width (1920 - 320)
    m_Editor_GameDest.h = WINDOW_HIEGHT; // Set height

    // [game mode] the start of the game position (render position)
    m_Game_GameDest.x = 0;
    m_Game_GameDest.y = 0;
    m_Game_GameDest.w = WINDOW_WIDTH;
    m_Game_GameDest.h = WINDOW_HIEGHT;

    coordinator.AddEventListener(EVENT::EVENT_ON_TILEMAP_CHANGED, [this](const Event& event){
        OnTileMapDataChanged(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_CAMERA_MOVE, [this](const Event& event){
        OnCameraMove(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_LEVEL_DATA_LOADED, [this](const Event& event){
        OnLevelDataLoaded(event);
    });
}

void RenderSystem::Render(){
    //coordinator.m_SDLGraphicsSystem->SetRenderDrawColor(110, 130,170,0xFF);
    // Clear the screen to the color of the empty framebuffer
    coordinator.m_SDLGraphicsSystem->RenderClear();

    if (coordinator.m_EndGame)
    {
        RenderWinPage();
        // coordinator.m_SDLGraphicsSystem->RenderPresent();
    }
    else
    {
        for (auto &entity : entityList)
        {

            // All sprite render
            if (coordinator.HashComponent<Sprite>(entity))
            {
                auto &sprite = coordinator.GetComponent<Sprite>(entity);

                // 开始界面渲染，如果有需要做的话。。。
                if (coordinator.m_State == GAME_STATE::MENU)
                {
                    if (coordinator.HashComponent<UIComponent>(entity))
                    {
                        auto &uiType = coordinator.GetComponent<UIComponent>(entity);
                        if (uiType.m_Type == UIType::MENU_IMAGE)
                        {
                            RenderSprite(sprite);
                        }
                    }

                    RenderStartBotton();
                }

                // UI 渲染管理，基于UI的UIType，在对应的场景中进行渲染
                if (coordinator.m_State == GAME_STATE::EDITOR)
                {
                    if (coordinator.HashComponent<UIComponent>(entity))
                    {
                        auto &uiType = coordinator.GetComponent<UIComponent>(entity);
                        if (uiType.m_Type == UIType::EDITOR_BUTTON)
                        {

                            RenderSprite(sprite);
                        }
                    }
                }

                // 游戏时渲染
                if (coordinator.m_State == GAME_STATE::GAME)
                {

                    // cout << "RenderSystem.cpp entered: coordinator.m_State == GAME_STATE::GAME " << endl;

                    if (coordinator.HashComponent<UIComponent>(entity))
                    {
                        auto &uiType = coordinator.GetComponent<UIComponent>(entity);
                        if (uiType.m_Type == UIType::GAME_IMAGE)
                        {
                            RenderSprite(sprite);
                        }
                    }

                    // if(coordinator.HashComponent<Player>(entity)){
                    //     RenderSprite(sprite);
                    // }

                    // ??
                    if (coordinator.HashComponent<Renderer>(entity))
                    {
                        RenderSprite(sprite);
                    }
                }
            }

            // All tilemap render: editor mode tilemap & game mode tilemap
            if (coordinator.HashComponent<Tilemap>(entity))
            {
                if (coordinator.m_State == GAME_STATE::EDITOR)
                {
                    // cout << "Rendering EDITOR" << endl;
                    RenderEditmap();
                    RenderTilemap(coordinator.m_Tilemap, m_Camera, m_Editor_GameDest);
                    RenderMouse(coordinator.m_Tilemap, m_Camera, m_Editor_GameDest);
                }
                else if (coordinator.m_State == GAME_STATE::GAME)
                {
                    // cout << "Rendering GAME" << endl;
                    RenderTilemap(coordinator.m_Tilemap, m_Camera, m_Game_GameDest);
                    RenderReturnBotton();
                }
            }
        }
    }

    coordinator.m_SDLGraphicsSystem->RenderPresent();
}

void RenderSystem::Update(){

}
void RenderSystem::RenderReturnBotton() {
    ImGui::NewFrame();
    ImGui::Begin("Return",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
    if (ImGui::Button("Return")) {
        coordinator.getSceneManager().EnterState(GAME_STATE::EDITOR);
    }
    ImGui::End();
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
    
}
void RenderSystem::RenderStartBotton() {
    
    ImGui::NewFrame();
    ImVec4 mario_red = ImVec4(0.992f, 0.149f, 0.204f, 1.0f);
    ImVec4 mario_yellow = ImVec4(0.996f, 0.898f, 0.325f, 1.0f);
    ImGui::SetNextWindowPos(ImVec2(660, 590));
    ImVec2 buttonSize = ImVec2(480, 100);
    ImGui::PushStyleColor(ImGuiCol_Button, mario_red);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, mario_yellow);
    ImGui::Begin("start",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |ImGuiWindowFlags_NoBackground);
    if (ImGui::Button("Start!", buttonSize)) {
        coordinator.getSceneManager().EnterState(GAME_STATE::EDITOR);
    }

    ImGui::PopStyleColor(2);
    ImGui::End();
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
}
void RenderSystem::RenderEditmap(){
    // Create the main window
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::NewFrame();
    ImGui::Begin("Editor Tool Window",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);


    // Create the tile component window
    ImGui::Text("Tile Component");
    ImGui::Separator();

    //print the tiles
    SDL_Surface* surface = IMG_Load(TILEMAP_BMP);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Render, surface);
    SDL_FreeSurface(surface);

    auto scrollY = ImGui::GetScrollY();
    auto scrollX = ImGui::GetScrollX();
    
    int imageWidth = 60 * 8;
    int imageHeight = 60 * 5;

    ImGui::Image(texture, ImVec2(imageWidth, imageHeight));

    // Set the button size and padding
    ImVec2 buttonSize = ImVec2(120, 80);
    ImVec2 buttonPadding = ImVec2(10, 5);

    // Create the buttons
    if (ImGui::Button("Start", buttonSize)) {
        coordinator.getSceneManager().EnterState(GAME_STATE::GAME);
    }
    ImGui::SameLine();
    if (ImGui::Button("Return", buttonSize)) {
        coordinator.getSceneManager().EnterState(GAME_STATE::MENU);
    }
    ImGui::SameLine();
    if (ImGui::Button("Extend", buttonSize)) {
        if (coordinator.m_State == GAME_STATE::EDITOR)
        {
            les->ConvertRawMapData(coordinator.m_Tilemap.m_Tiles, 5);
        }
    }
    if (ImGui::Button("Win", buttonSize)) {
        if (coordinator.m_State == GAME_STATE::EDITOR)
        {
            coordinator.m_EndGame = true;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Save", buttonSize)) {
        if (coordinator.m_State == GAME_STATE::EDITOR)
        {
            coordinator.m_SaveGame = true;
            cout << "Save button is clicked: " << coordinator.m_SaveGame << endl;
            FileReader::GetInstance().SaveTilemapText(coordinator.m_Tilemap.m_MapX, coordinator.m_Tilemap.m_MapY, coordinator.m_Tilemap.m_Tiles, TILEMAP_TXT);
        }
    }

    // End the main window
    ImGui::End();
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
}

void RenderSystem::RenderWinPage()
{
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(260, 440));
    ImGui::SetNextWindowSize(ImVec2(1250, 200));
    ImGui::Begin("You Win!", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Congratulations, you won the game!"); // Draw the trophy icon with yellow color
    ImGui::SetWindowFontScale(5.0f);
    // Draw a button to play again
    ImVec2 buttonSize(400.0f, 70.0f);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonSize.x) / 2); // Center the button horizontally
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
    if (ImGui::Button("Play Again", buttonSize))
    {
        coordinator.m_EndGame = false;
        coordinator.getSceneManager().EnterState(GAME_STATE::EDITOR);
    }

    ImGui::End();
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
}

void RenderSystem::RenderMouse(Tilemap& tilemap, SDL_Rect src, SDL_Rect dest){
    int cursorTile = coordinator.m_LevelEditorSystem->getSelectedTile(); 
    if(cursorTile >= 0) {
        int x = (cursorTile % tilemap.m_Cols) * (tilemap.m_TileWidth + tilemap.m_gap);
        int y = (cursorTile / tilemap.m_Rows) * (tilemap.m_TileHeight + tilemap.m_gap);
        int w = tilemap.m_TileWidth;
        int h = tilemap.m_TileHeight;
        SDL_Rect Rect = {x, y, w, h};
        
        int cursorW = tilemap.m_TileWidth * tilemap.m_ScaleFactor;
        int cursorH = tilemap.m_TileHeight * tilemap.m_ScaleFactor;
        int cursorX = ImGui::GetMousePos().x - cursorW/2;
        int cursorY = ImGui::GetMousePos().y - cursorH/2;
        SDL_Rect cursorRect = { cursorX, cursorY, cursorW, cursorH};

        SDL_RenderCopy(m_Render, tilemap.m_Texture, &Rect, &cursorRect);
    }  
}


void RenderSystem::RenderTilemap(Tilemap& tilemap, SDL_Rect src, SDL_Rect dest){
    SDL_RenderCopy(m_Render, tilemap.m_FullTexture, &src, &dest);
}

void RenderSystem::RenderSprite(Sprite& sprite){
    SDL_RenderCopy(m_Render, sprite.m_Texture, &sprite.m_Src, &sprite.m_Dest);
}

int RenderSystem::GetTileType(int x, int y, Tilemap& tilemap){
    return tilemap.m_Tiles[y * tilemap.m_MapX + x];
}

void RenderSystem::OnTileMapDataChanged(const Event &event)
{
    m_TextureWidth = coordinator.m_Tilemap.m_MapX * coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_ScaleFactor;
    m_TextureHeight = coordinator.m_Tilemap.m_MapY * coordinator.m_Tilemap.m_TileHeight * coordinator.m_Tilemap.m_ScaleFactor;
    // cout << "RenderSystem, tilemap's mapX is "<< coordinator.m_Tilemap.m_MapX << endl;
    CreateTilemapTexture(coordinator.m_Tilemap);
}

void RenderSystem::OnLevelDataLoaded(const Event &event)
{
    // coordinator.m_Tilemap = event.GetParameter<Tilemap>("mapObject");
    // 在读取文件后，创建一个巨大的地图texture，之后有修改就修改这个texture
    m_TextureWidth = coordinator.m_Tilemap.m_MapX * coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_ScaleFactor;
    m_TextureHeight = coordinator.m_Tilemap.m_MapY * coordinator.m_Tilemap.m_TileHeight * coordinator.m_Tilemap.m_ScaleFactor;
    CreateTilemapTexture(coordinator.m_Tilemap);
}

void RenderSystem::EnterState(const GAME_STATE &state)
{
    cout << "Entering RenderSystem" << endl;
    coordinator.m_State = state;
    switch (state)
    {
    case GAME_STATE::EDITOR:
        // 重新赋值渲染窗口
        coordinator.m_SaveGame = false;
        m_Camera.x = 0;
        m_Camera.y = 0;
        m_Camera.w = (WINDOW_WIDTH - EDITOR_WIDTH);
        m_Camera.h = WINDOW_HIEGHT;

        m_Editor_GameDest.x = EDITOR_WIDTH;
        m_Editor_GameDest.y = 0;
        m_Editor_GameDest.w = WINDOW_WIDTH - EDITOR_WIDTH; // Set width (1920 - 320)
        m_Editor_GameDest.h = WINDOW_HIEGHT;               // Set height

        m_TextureWidth = coordinator.m_Tilemap.m_MapX * coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_ScaleFactor;
        m_TextureHeight = coordinator.m_Tilemap.m_MapY * coordinator.m_Tilemap.m_TileHeight * coordinator.m_Tilemap.m_ScaleFactor;
        break;
    case GAME_STATE::GAME:
        m_Camera.x = 0;
        m_Camera.y = 0;
        m_Camera.w = WINDOW_WIDTH;
        m_Camera.h = WINDOW_HIEGHT;

        m_Game_GameDest.x = 0;
        m_Game_GameDest.y = 0;
        m_Game_GameDest.w = WINDOW_WIDTH;
        m_Game_GameDest.h = WINDOW_HIEGHT;

        m_TextureWidth = coordinator.m_Tilemap.m_MaxMapWidth;
        m_TextureHeight = coordinator.m_Tilemap.m_MapY * coordinator.m_Tilemap.m_TileHeight * coordinator.m_Tilemap.m_ScaleFactor;
    default:
        break;
    }
}

void RenderSystem::Clear()
{
    for(auto& entity : m_Entities){
        coordinator.getEntityManager().DestroyEntity(entity);
        cout << "Destroying entity: " << entity << endl;
    }
    m_Entities.clear();
    entityList.clear();
}

void RenderSystem::CreateTilemapTexture(Tilemap &tilemap)
{
    /*
        如何将每个单独渲染的tile放到一个大的texture中的方法：

        创建一个大的纹理，其大小等于tilemap的大小。
        设置渲染目标为创建的大纹理。
        遍历tilemap，将每个tile绘制到纹理上。
        将渲染目标重置为默认的渲染目标（窗口）。
    */
    // 1. calculate how big the texture is || 创建一个大的纹理，其大小等于tilemap的大小
    
    tilemap.m_FullTexture = SDL_CreateTexture(
        m_Render, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        m_TextureWidth, 
        m_TextureHeight);

    // 2. set the target renderer as m_TilemapTexture || 设置渲染目标为创建的大纹理。
    SDL_SetRenderTarget(m_Render, tilemap.m_FullTexture);

    SDL_SetRenderDrawColor(m_Render, 110, 130, 170, 0xFF);
    SDL_RenderClear(m_Render);

    // 3. || 遍历tilemap，将每个tile绘制到纹理上。
    // std::cout << "CreateTilemapTexture here: " << endl;
    for (int y = 0; y < tilemap.m_MapY; y++)
    {
        for (int x = 0; x < tilemap.m_MapX; x++)
        {
            // Select our Tile
            int currentTile = GetTileType(x, y, tilemap);
            if (currentTile > -1)
            {

                tilemap.m_Src.x = (currentTile % tilemap.m_Cols) * (tilemap.m_TileWidth + tilemap.m_gap);
                tilemap.m_Src.y = (currentTile / tilemap.m_Rows) * (tilemap.m_TileHeight + tilemap.m_gap);
                tilemap.m_Src.w = tilemap.m_TileWidth;
                tilemap.m_Src.h = tilemap.m_TileHeight;

                tilemap.m_Dest.x = x * tilemap.m_TileWidth * tilemap.m_ScaleFactor;
                tilemap.m_Dest.y = y * tilemap.m_TileHeight * tilemap.m_ScaleFactor;
                tilemap.m_Dest.w = tilemap.m_TileWidth * tilemap.m_ScaleFactor;
                tilemap.m_Dest.h = tilemap.m_TileHeight * tilemap.m_ScaleFactor;

                SDL_RenderCopy(m_Render, tilemap.m_Texture, &tilemap.m_Src, &tilemap.m_Dest);
            }
        }
    }

    // set the render target as default(window)
    SDL_SetRenderTarget(m_Render, nullptr);
}


void RenderSystem::AddEntity(Entity entity){
    entityList.push_back(entity);
    SortRenderOrder();
}

void RenderSystem::SortRenderOrder(){
    std::sort(entityList.begin(), entityList.end(), [](const Entity& a, const Entity& b) {
        auto& rendererA = coordinator.GetComponent<Renderer>(a);
        auto& rendererB = coordinator.GetComponent<Renderer>(b);
        return rendererA.renderPriority < rendererB.renderPriority;
    });
}

void RenderSystem::OnCameraMove(const Event &event)
{
    m_Camera.x = event.GetParameter<int>("cameraX");
}