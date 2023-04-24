#include "ECSCoordinator.hpp"
#include "LevelEditorSystem.hpp"

extern ECSCoordinator coordinator;

void LevelEditorSystem::Init(){
    // register mouse event
    coordinator.AddEventListener(EVENT::EVENT_ON_MOUSE_CLICKED_LEFT, [this](const Event& event){
        OnMouseClicked_Left(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_MOUSE_CLICKED_RIGHT, [this](const Event& event){
        OnMouseClicked_Right(event);
    });

    // register get level data event
    coordinator.AddEventListener(EVENT::EVENT_ON_LEVEL_DATA_LOADED, [this](const Event& event){
        OnLevelDataLoaded(event);
    });

    coordinator.AddEventListener(EVENT::EVENT_ON_CAMERA_MOVE, [this](const Event& event){
        OnCameraMove(event);
    });
}

void LevelEditorSystem::Update(){

}

void LevelEditorSystem::Clear()
{
    if (m_Entities.size() > 0)
    {

        for (auto &entity : m_Entities)
        {
            coordinator.getEntityManager().DestroyEntity(entity);
        }
        m_Entities.clear();
    }
}

void LevelEditorSystem::OnMouseClicked_Left(const Event &event)
{
    if (coordinator.m_State == GAME_STATE::EDITOR)
    {
        
        m_MouseX = event.GetParameter<int>("mouseX");
        m_MouseY = event.GetParameter<int>("mouseY");
 
        int tileX = (m_MouseX) / (m_TilePixel);
        int tileY = (m_MouseY) / (m_TilePixel);
        // 该offset是editor占的多余的像素，需要去掉
        int offset = EDITOR_WIDTH / m_TilePixel;

        PutTile(m_MouseX, m_MouseY, tileX, tileY, m_CameraOffset, offset, tileType);
    }
}

void LevelEditorSystem::OnMouseClicked_Right(const Event &event)
{
    if (coordinator.m_State == GAME_STATE::EDITOR)
    {

        // m_MouseX = event.GetParameter<int>("mouseX");
        // m_MouseY = event.GetParameter<int>("mouseY");

        // int tileX = static_cast<int>(std::floor(static_cast<float>(m_MouseX) / m_TilePixel));
        // int tileY = m_MouseY / m_TilePixel;

        // int offset = EDITOR_WIDTH / m_TilePixel;
        
        //PutTile(m_MouseX, m_MouseY, tileX, tileY, m_CameraOffset, offset, -1);
        //switch to ease mode.
        tileType = -1;
    }
}

void LevelEditorSystem::OnCameraMove(const Event &event)
{
    m_CameraSpeed = event.GetParameter<int>("cameraSpeed");
    m_CameraOffset = event.GetParameter<int>("cameraX");
    // cout << "m_CameraOffset is " << m_CameraOffset << endl;
}

void LevelEditorSystem::OnLevelDataLoaded(const Event &event)
{
    m_LevelData = coordinator.m_Tilemap.m_Tiles;
    m_TilePixel = coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_ScaleFactor;
    m_MaxMapWidth = coordinator.m_Tilemap.m_MapX * coordinator.m_Tilemap.m_TileWidth * coordinator.m_Tilemap.m_ScaleFactor;
}

void LevelEditorSystem::OnKeybardClicked_O(const Event &event)
{
    // cout << "Saving data now!" << endl;
    // FileReader::GetInstance().SaveTilemapText(coordinator.m_Tilemap.m_MapX, coordinator.m_Tilemap.m_MapY, coordinator.m_Tilemap.m_Tiles, TILEMAP_TXT);
    if (coordinator.m_State == GAME_STATE::EDITOR)
    {
    }
    // TODO: 之后将这个移动进上面的判断。
    coordinator.getSceneManager().EnterState(GAME_STATE::EDITOR);
}

void LevelEditorSystem::OnKeybardClicked_I(const Event &event)
{
    // FileReader::GetInstance().SaveTilemapText(coordinator.m_Tilemap.m_MapX, coordinator.m_Tilemap.m_MapY, coordinator.m_Tilemap.m_Tiles, TILEMAP_TXT);
    // DispatchChangeStatusRequest(GAME_STATE::GAME);
    // TODO: 问题：游戏与编辑器切换后，编辑器本身的功能遭到破坏
    if (coordinator.m_State == GAME_STATE::EDITOR)
    {
        coordinator.getSceneManager().EnterState(GAME_STATE::GAME);
    }
}

void LevelEditorSystem::OnKeybardClicked_U(const Event &event)
{
    if (coordinator.m_State == GAME_STATE::EDITOR)
    {
        ConvertRawMapData(coordinator.m_Tilemap.m_Tiles, 5);
    }
}

void LevelEditorSystem::DispatchTileMapChangedData(const Tilemap &tilemap)
{
    Event event(EVENT::EVENT_ON_TILEMAP_CHANGED);
    coordinator.TriggerEvent(event);
}

bool LevelEditorSystem::IsMouseWintinGameBoundary(int tileX, int tileY)
{
    // 320 / (16 * 4) = 5, so we can put the map on the (5, 0 - maxY) position(minimun position)
    int minX = EDITOR_WIDTH / m_TilePixel;
    int maxX = coordinator.m_Tilemap.m_MaxMapWidth / m_TilePixel;
    int maxY = WINDOW_HIEGHT / m_TilePixel;

    return tileX >= minX && tileX <= maxX && tileY >= 0 && tileY <= maxY;
}

bool LevelEditorSystem::IsMouseWintinEditorBoundary(int mouseX, int mouseY)
{
    int toolMap_X = 70;
    int toolMap_Y = 90;
    int tileSize = 60;
    int toolMap_XEnd = toolMap_X + 8 * tileSize;
    int toolMap_YEnd = toolMap_Y + 6 * tileSize;
    int tileX = (m_MouseX - toolMap_X) / (tileSize);
    int tileY = (m_MouseY - toolMap_Y) / (tileSize);
    int maxX = EDITOR_WIDTH / m_TilePixel;
    int maxY = WINDOW_HIEGHT / m_TilePixel;

    Debug::PrintMessage("Mouse is under editor boundary");
    cout << tileX << ", " << tileY << endl;

    if(tileX >= 0 && tileX < maxX && tileY >= 0 && tileY < maxY) {
        if(mouseX >= toolMap_X && mouseX <= toolMap_XEnd && mouseY >= toolMap_Y && mouseY <= toolMap_YEnd){
            tileType = tileX + tileY*8;
            cout << tileType << endl;   
        }
        return true;
    }
    return false;
}

int LevelEditorSystem::GetTileType(int x, int y)
{
    return coordinator.m_Tilemap.m_Tiles[y * coordinator.m_Tilemap.m_MapX + x];
}

void LevelEditorSystem::SetTile(int x, int y, int type)
{
    int index = y * coordinator.m_Tilemap.m_MapX + x;
    if (index > coordinator.m_Tilemap.m_Tiles.size())
    {
        cout << "index is " << index << endl;
        cout << "m_LevelData.size() is " << coordinator.m_Tilemap.m_Tiles.size() << endl;
        cout << "Set tile index out of boundary" << endl;
    }
    else
    {
        if(type == GetTileType(x, y)){
            cout << "this tile is the same as you clicked!" << GetTileType(x,y);
            return;
        }

        cout << "m_LevelData's size is: " << m_LevelData.size() << endl;

        coordinator.m_Tilemap.m_Tiles[index] = type;

        // send map data to Render system
        DispatchTileMapChangedData(coordinator.m_Tilemap);
        cout << "Modify tile type: " << index << endl;
        // this->Print();
    }
}

void LevelEditorSystem::PutTile(int mouseX, int mouseY, int tileX, int tileY, int cameraOffset, int screenOffset, int type)
{
    if(IsMouseWintinGameBoundary(tileX, tileY)){
        // 有偏移的情况，加上摄像机的偏移
        if(m_CameraOffset != 0 && m_CameraSpeed != 0){
            m_MouseX += m_CameraOffset;
            tileX = static_cast<int>(std::floor(static_cast<float>(m_MouseX) / m_TilePixel));
            // cout << m_MouseX << ", " << m_MouseY << endl;
            // cout << "tileX" << tileX << endl;
            // 减掉偏移量，才能找到在地图上的正确值，例如加上偏移是34，但地图最大只有30，此时要减去偏移量
            this->SetTile(tileX - screenOffset, tileY, type);
            // cout << "SetTile tileX" << tileX + m_CameraOffset / m_TilePixel << endl;
        }
        // 没有偏移的情况，正常计算
        else{
            this->SetTile(tileX - screenOffset, tileY, type);
            // cout << m_MouseX << ", " << m_MouseY << endl;
        }
        
        Debug::PrintMessage("Mouse is under game boundary");
    }
    else if(IsMouseWintinEditorBoundary(mouseX, mouseY))
    {
        Debug::PrintMessage("Mouse is under editor boundary");
    }
    else{
        cout << "Editor position out of boundary !!!!" << endl;
    }
}

/// @brief 这块有点设计失误了，map应该设置2维的，现在是1维，转换来转换去。。。先不改咯~
/// @param m_LevelData 
/// @param size 
void LevelEditorSystem::ConvertRawMapData(vector<int> m_LevelData, int size)
{
    int height = 17;
    int width = m_LevelData.size() / height;
    cout << "ConvertRawMapData  " << width << endl;

    // Convert 1D to 2D
    vector<vector<int>> tempVector(height, vector<int>(width));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tempVector[i][j] = m_LevelData[i * width + j];
            cout << tempVector[i][j] << " ";
        }
        cout << endl;
    }

    // Add n (-1) values at the end of each row
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < size; j++)
        {
            tempVector[i].push_back(-1);
        }
    }

    cout << "added -1 to the end " << endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < tempVector[0].size(); j++)
        {
            cout << tempVector[i][j] << " ";
        }
        cout << endl;
    }

    // Convert back to 1D
    vector<int> tempNewMapData(height * (width + size));
    cout << "tempVector's: " << tempNewMapData.size() << endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width + size; j++)
        {
            // 注意这里要+size，否则会内存越界，输出奇怪的东西
            tempNewMapData[i * (width + size) + j] = tempVector[i][j];
            cout << tempNewMapData[i * (width + size) + j] << " ";
        }
        cout << endl;
    }

    cout << "coordinator.m_Tilemap.m_Tiles's: " << endl;
    coordinator.m_Tilemap.m_Tiles            = tempNewMapData; 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width + size; j++)
        {
            // tempNewMapData[i * (width) + j] = tempVector[i][j];
            cout << coordinator.m_Tilemap.m_Tiles[i * (width + size) + j] << " ";
        }
        cout << endl;
    }

    // 需要修改的内容：1、地图的原始数据 2、地图宽度 3、摄像头的移动范围 4、重新渲染地图
    coordinator.m_Tilemap.m_MaxMapWidth     += coordinator.m_Tilemap.m_ScaleFactor * size * coordinator.m_Tilemap.m_TileWidth;
    coordinator.m_Tilemap.m_MapX            += size;
    coordinator.m_TempTilemap                = coordinator.m_Tilemap;
    cout << "Transforming data to other events now...." << endl;
    DispatchTileMapChangedData(coordinator.m_Tilemap);
}

void LevelEditorSystem::Print()
{
    for (int y = 0; y < coordinator.m_Tilemap.m_MapY; y++)
    {
        for (int x = 0; x < coordinator.m_Tilemap.m_MapX; x++)
        {
            std::cout << " " << GetTileType(x, y);
        }
        std::cout << "\n";
    }
}

int LevelEditorSystem::getSelectedTile() {
    return tileType;
}
