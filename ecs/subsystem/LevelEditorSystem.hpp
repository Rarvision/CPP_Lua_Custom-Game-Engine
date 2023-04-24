#pragma once

#include "System.hpp"
#include "Event.hpp"

#include <vector>

using namespace std;

class LevelEditorSystem: public ISystem{
public:
    void Init() override;   
    
    void Update() override;

    void Clear() override;

    int getSelectedTile();  
    
    /// @brief Expand the map with the size
    /// @param m_LevelData 1d map array
    /// @param size tiles you want to add you the map, if you set 5 and the tile size is 64, you add 320 pixels to the right of the map
    void ConvertRawMapData(vector<int> m_LevelData, int size);

private:
    
    void OnMouseClicked_Left(const Event& event);

    void OnMouseClicked_Right(const Event& event);

    void OnCameraMove(const Event &event);

    void OnLevelDataLoaded(const Event& event);

    void OnKeybardClicked_O(const Event& event);

    void OnKeybardClicked_I(const Event& event);

    void OnKeybardClicked_U(const Event& event);

    /// @brief dispatch changed tilemap data to subscribers
    /// @param tilemap tilemap obejct
    void DispatchTileMapChangedData(const Tilemap& tilemap);

    /// @brief [editor] check if the mouse is within the tilemap boundary
    /// @param x mouse x
    /// @param y mouse y
    /// @return 
    bool IsMouseWintinGameBoundary(int x, int y);

    /// @brief [editor] check if the mouse is within the editor boundary
    /// @param x mouse x
    /// @param y mouse y
    /// @return 
    bool IsMouseWintinEditorBoundary(int x, int y);

    /// @brief set a tile based on the tile type
    /// @param x tile pos x in tilemap array
    /// @param y tile pos y in tilemap array
    /// @param type tile id you want to set here
    void SetTile(int x, int y, int type);

    /// @brief put tiles based on the mouse position
    /// @param mouseX
    /// @param mouseY 
    /// @param tileX tileX = mouse x position / tile pixel size
    /// @param tileY tiley = mouse y position / tile pixel size
    /// @param cameraOffset camera always moves in [editor], we add this to calculate the right position in tile array
    /// @param screenOffset 
    /// @param tileType tile id you want to set here
    void PutTile(int mouseX, int mouseY, int tileX, int tileY, int cameraOffset, int screenOffset, int tileType);


    /// @brief calculate map data based on the 2d array
    // void CalculateRenderedMapData();

    void Print();

    /// @brief get the tile type based on the tile x and y
    /// @param x tile x
    /// @param y tile y
    /// @return an id of the tile
    int GetTileType(int x, int y); 

    // save mouse position
    int                 m_MouseX;
    int                 m_MouseY;
    
    // left side is editor, right side is game
    int                 m_GameAreaWidth;
    // int             m_Max_Tile_Width_Count;
    // int             m_Max_Tile_Height_Count;

    // save level data
    vector<int>         m_LevelData;
    vector<vector<int>> m_TwoDMapData;
    int                 m_TilePixel;
    int                 m_CameraOffset = 0;
    int                 m_CameraSpeed  = 0;
    int                 m_MaxMapWidth;
    int                 tileType = 0;
};