#include "System.hpp"
#include "LevelEditorSystem.hpp"
#include <vector>

using namespace std;

// 现在的m_Entity在RenderSystem中毫无作用，原因是我需要基于entity中的Render组件的值进行排序，但是我真不知道怎么写。。。有空再改
// class EntityRenderPriorityComparator {
// public:
//     set<Entity, EntityRenderPriorityComparator> m_SortedEntities;

//     bool operator()(const Entity& a, const Entity& b) const {
//         auto& rendererA = coordinator.GetComponent<Renderer>(a);
//         auto& rendererB = coordinator.GetComponent<Renderer>(b);
//         return rendererA.renderPriority < rendererB.renderPriority;
//     }
// };

class RenderSystem : public ISystem{
public:
	void Init() override;

	void Update() override;

    void Clear() override;
    
    void Render();
    
    void EnterState(const GAME_STATE& state);

    /// @brief [This function is not elegant!] Order entites based on render priority
    /// @param entity entites that require render
    void AddEntity(Entity entity);

private:
    std::unique_ptr<class LevelEditorSystem> les;

    /// @brief Get the latest tilemap data when this event is triggered
    /// @param event 
    void OnTileMapDataChanged(const Event& event);

    void OnCameraMove(const Event &event);

    void OnLevelDataLoaded(const Event& event);

    /// @brief we render objects from big to small, so we sort render order here
    void SortRenderOrder();

    void RenderStartBotton();

    void RenderReturnBotton();
    
    void RenderEditmap();

    void RenderWinPage();
    void RenderMouse(Tilemap& tilemap, SDL_Rect src, SDL_Rect dest);

    void RenderTilemap(Tilemap& tilemap, SDL_Rect src, SDL_Rect dest);

    void RenderSprite(Sprite& sprite);

    void CreateTilemapTexture(Tilemap& tilemap);

    // this function is not working
    // void UpdateTilemapTexture(Tilemap& tilemap);

    int GetTileType(int x, int y, Tilemap& tilemap);

    SDL_Renderer*   m_Render            = nullptr;
    SDL_Window*     m_Window            = nullptr;

    SDL_Texture*    m_TilemapTexture    = nullptr;

    /// @brief stores entities need to be rendered
    vector<Entity>  entityList          = {};

    /// @brief the render area in render system
    SDL_Rect        m_Camera;
    /// @brief where to render screen in editor
    SDL_Rect        m_Editor_GameDest;
    /// @brief where to render screen in game
    SDL_Rect        m_Game_GameDest;

    int             m_TextureWidth;
    int             m_TextureHeight;
};