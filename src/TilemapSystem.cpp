#include "ECSCoordinator.hpp"

extern ECSCoordinator coordinator;

void TilemapSystem::Init()
{
    // TODO: 晚点用事件传递Render
    m_Render = coordinator.m_SDLGraphicsSystem->GetSDLRenderer();
    
    Debug::PrintMessage("Init TilemapSystem");
}

void TilemapSystem::Update()
{
}
void TilemapSystem::Clear()
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

void TilemapSystem::EnterState(const GAME_STATE &state)
{
    coordinator.m_State = state;
    switch (state)
    {
    case GAME_STATE::GAME:
        // cout << "Entering tilemap system in GAME" << endl
        
        if (!coordinator.m_SaveGame)
        {
            cout << "XXXXXXXX do not change tilemap data" << endl;
            coordinator.m_Tilemap = coordinator.m_TempTilemap;
        }
        break;
    default:
        break;
    }
}

TilemapSystem::~TilemapSystem()
{
    if(m_TilemapTexture != nullptr){
        SDL_DestroyTexture(m_TilemapTexture);
        m_TilemapTexture = nullptr;
    }
}

Tilemap TilemapSystem::CreateTileMap(string tileSheetFileName, int _rows, int _cols, int _TileWidth, int _TileHeight, int _mapX, int _mapY, int _scaleFactor, vector<int>& m_TileMapVector, int _gap)
{
    Tilemap _tileMap;
    coordinator.m_Tilemap = _tileMap;
    if (nullptr == m_Render)
    {
        SDL_Log("No valid renderer found");
    }

    // Setup variables
    coordinator.m_Tilemap.m_Rows          = _rows;
    coordinator.m_Tilemap.m_Cols          = _cols;
    coordinator.m_Tilemap.m_TileWidth     = _TileWidth;
    coordinator.m_Tilemap.m_TileHeight    = _TileHeight;
    coordinator.m_Tilemap.m_MapX          = _mapX;
    coordinator.m_Tilemap.m_MapY          = _mapY;
    coordinator.m_Tilemap.m_ScaleFactor   = _scaleFactor;
    coordinator.m_Tilemap.m_Tiles         = m_TileMapVector;
    coordinator.m_Tilemap.m_MaxMapWidth   = _mapX * _scaleFactor * _TileWidth;
    coordinator.m_Tilemap.m_gap           = _gap;
    coordinator.m_Tilemap.m_TileSpriteSheet = ResourceManager::GetInstance().GetSDL_Surface(tileSheetFileName.c_str());

    if (nullptr == coordinator.m_Tilemap.m_TileSpriteSheet)
    {
        SDL_Log("Failed to allocate surface");
    }
    else
    {
        coordinator.m_Tilemap.m_Texture = ResourceManager::GetInstance().GetSDL_Texture(tileSheetFileName.c_str(), m_Render, coordinator.m_Tilemap.m_TileSpriteSheet);
    }

    this->DispatchLevelData(m_TileMapVector);
    
    coordinator.m_TempTilemap = coordinator.m_Tilemap;

    return coordinator.m_Tilemap;
}

int TilemapSystem::GetTileType(int x, int y)
{
    return coordinator.m_Tilemap.m_Tiles[y * coordinator.m_Tilemap.m_MapX + x];
}

void TilemapSystem::DispatchLevelData(vector<int> &levelData)
{
    Event event(EVENT::EVENT_ON_LEVEL_DATA_LOADED);

    coordinator.TriggerEvent(event);
}

// void TilemapSystem::PrintMap()
// {
//     Debug::PrintMessage("Printint map: ");
//     for (int y = 0; y < coordinator.m_Tilemap.m_MapY; y++)
//     {
//         for (int x = 0; x < coordinator.m_Tilemap.m_MapX; x++)
//         {
//             std::cout << " " << GetTileType(x, y);
//         }
//         std::cout << "\n";
//     }
// }