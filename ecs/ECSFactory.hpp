#pragma once

#include "ECSCoordinator.hpp"

extern ECSCoordinator coordinator;

class ECSFactory{
public:
    ECSFactory();

    /// @brief init all components for the system
    void InitComponents();

    void InitMenu();

    void InitEditor();

    void InitGame();

    bool m_IsInitEditorSystem;

    bool m_IsInitGameSystem;

    bool m_IsInitComponents;

    bool m_IsInitCommonEntity;

    bool m_IsInitCamera;

    bool m_IsRegisterMenuSystem;

    bool m_IsRegisterMenuEntity;

private:
    void CreateGameManager();
    
    void CreateTileMapEntities();

    void CreateGroundEntity(); 

    void CreateTestEntityFromJSON();

    void CreateTestEntitySecondFromJSON();

    void CreatePlayerFromJSON();

    void CreateTilemapFromJSON();

    void CreateEnemyFromJSON();

    void CreateCamera();

    void AddEntity(ENTITY_TYPE);

    void AddEnemyToJSON(Enemy enemyData);

    void CreateMenuUI();

    void CreateEditorUI();

    /// @brief this method could create all UI Types from JSON
    /// @param entity 
    /// @param btnName your UI name in JSON
    /// @param buttonConfig your JSON Config
    /// @param state which game state do you want to show this UI
    /// @param uiType uiType of the UI
    /// @param renderOrder render order, see Common.hpp
    void CreateUIElement(Entity entity, const string& btnName, const json& buttonConfig, GAME_STATE state, UIType uiType, RENDER_ORDER renderOrder);

    /// @brief register common system for game and editor
    void RegisterCommonSystem();

    void RegisterMenuSystem();

    void RegisterGameSystem();
    
    void RegisterMenuEntity();

    /// @brief register common entity for game and editor
    void RegisterCommonEntity();

    void CleanBrickTransform();

    void CleanBrickEntities();

    void CleanBrickB2Body();

    void CleanGroundEntities();

    bool m_IsUIConfigFileExist;

    bool m_IsConfigFileExist;

    json m_Config;
    json m_ImageConfig;
    json m_ButtonConfig;
    json m_CameraConfig;
    // json m_ComponentConfig;

    vector<Entity> m_BrickEntities;
    vector<Entity> m_GroundEntities;
    vector<Entity> m_B2BEntities;
    vector<Entity> m_BrickTransform;
};