#include "System.hpp"
#include "Tilemap.hpp"
#include "Event.hpp"

#include <vector>

using namespace std;

class TilemapSystem : public ISystem{
public:
	void Init() override;

	void Update() override;

    void Clear() override;

    void EnterState(const GAME_STATE &state);

    Tilemap CreateTileMap(string tileSheetFileName, int rows, int cols, int TileWidth, int TileHeight, int mapX, int mapY, int scaleFactor, vector<int>& m_TileMapVector, int gap);

    ~TilemapSystem();

private:

    // void PrintMap();

    int GetTileType(int x, int y);

    /// @brief send level data to subscribers
    /// @param levelData 
    void DispatchLevelData(vector<int>& levelData);

    /// @brief Store tilemap object from JSON
    vector<vector<int>>     m_TileMapVector                 = {};
    SDL_Renderer*           m_Render                        = nullptr;
    SDL_Texture*            m_TilemapTexture                = nullptr;
};