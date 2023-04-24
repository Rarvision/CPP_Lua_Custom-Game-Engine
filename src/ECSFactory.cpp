#include "ECSFactory.hpp"

ECSFactory::ECSFactory() : m_IsInitEditorSystem(false), m_IsInitComponents(false), m_IsInitCommonEntity(false), m_IsConfigFileExist(false)
{
    m_IsUIConfigFileExist = FileReader::GetInstance().LoadJSONFile(UI_CONFIG);
    m_IsConfigFileExist = FileReader::GetInstance().LoadJSONFile(COMPONENT_CONFIG);

    if(m_IsUIConfigFileExist)
    {
        m_Config = FileReader::GetInstance().GetConfig(UI_CONFIG);
        m_ImageConfig = m_Config["Images"];
        m_ButtonConfig = m_Config["Buttons"];
    }

    if(m_IsConfigFileExist){
        m_Config = FileReader::GetInstance().GetConfig(COMPONENT_CONFIG);
        m_CameraConfig = m_Config["Camera"];
    }
}

void ECSFactory::InitComponents(){
    // ///////////////////////////////////////////
    // //         set up component here         //
    // ///////////////////////////////////////////
    coordinator.RegisterComponent<Transform>();
    coordinator.RegisterComponent<Player>();
    coordinator.RegisterComponent<Sprite>();
    coordinator.RegisterComponent<Tilemap>();
    coordinator.RegisterComponent<Renderer>();
    coordinator.RegisterComponent<Enemy>();
    coordinator.RegisterComponent<InPut>();
    coordinator.RegisterComponent<Editor>();
    coordinator.RegisterComponent<UIComponent>();
    coordinator.RegisterComponent<Camera>();
    coordinator.RegisterComponent<Name>();
    coordinator.RegisterComponent<Tag>();
    coordinator.RegisterComponent<Script>();
    coordinator.RegisterComponent<PhysicsBody>();
    coordinator.RegisterComponent<Tile>();
}

void ECSFactory::InitMenu()
{
    if (!this->m_IsInitComponents)
    {
        this->InitComponents();
        this->m_IsInitComponents = true;
    }

    if (!this->m_IsRegisterMenuSystem)
    {
        RegisterMenuSystem();
        this->m_IsRegisterMenuSystem = true;
    }
    
    RegisterMenuEntity();
}

void ECSFactory::InitEditor()
{
    if (!this->m_IsInitEditorSystem)
    {
        RegisterCommonSystem();
        this->m_IsInitEditorSystem = true;
    }

    if (!this->m_IsInitCamera)
    {
        CreateCamera();
        this->m_IsInitCamera = true;
    }

    RegisterCommonEntity();

}

void ECSFactory::InitGame()
{
    if (!this->m_IsInitGameSystem)
    {
        CreateCamera();
        this->m_IsInitCamera = true;
    }

    if(!this->m_IsInitGameSystem){

        RegisterGameSystem();

        this->m_IsInitGameSystem = true;
    }

    CreateCamera();
    RegisterCommonEntity();
    CreatePlayerFromJSON();
    // CreateTestEntityFromJSON();
    // CreateTestEntitySecondFromJSON();
    CreateTileMapEntities();
    // CreateGroundEntity();
}

void ECSFactory::CreateGameManager()
{
    Entity gameManager;
    coordinator.AddComponent(gameManager, Transform{Vec2D(0.0f, 0.0f)});
    coordinator.AddComponent(gameManager, Tag{"GameManager"});
    coordinator.AddComponent(gameManager, Name{"GameManager"});
    coordinator.AddComponent(gameManager, Script{"game_manager"});
}

void ECSFactory::CreateTileMapEntities()
{
    CleanBrickTransform();
    CleanBrickEntities();
    CleanBrickB2Body();
    Tilemap& tilemap = coordinator.m_Tilemap;

    for (int y = 0; y < tilemap.m_MapY; y++)
    {
        for (int x = 0; x < tilemap.m_MapX; x++)
        {
            // Select our Tile
            int currentTile = tilemap.m_Tiles[y * tilemap.m_MapX + x];
            if (currentTile > -1)
            {
                Entity entity;
                entity = coordinator.CreateEntity();

                coordinator.AddComponent(entity, Transform{Vec2D(
                    x * tilemap.m_TileWidth * tilemap.m_ScaleFactor,y * tilemap.m_TileHeight * tilemap.m_ScaleFactor)});
                coordinator.AddComponent(entity, Tile{Vec2D{x, y}});
                m_BrickTransform.push_back(entity);

                if(currentTile == 22)
                {
                    coordinator.AddComponent(entity, Tag{"Coin"});
                    coordinator.AddComponent(entity, PhysicsBody{
                    coordinator.m_PhysicsManagerSystem->CreateB2Body(entity, STATIC_SENSOR)});
                } else if(currentTile == 31)
                {
                    coordinator.AddComponent(entity, Tag{"Trophy"});
                    coordinator.AddComponent(entity, PhysicsBody{
                    coordinator.m_PhysicsManagerSystem->CreateB2Body(entity, STATIC_SENSOR)});
                } else {
                    coordinator.AddComponent(entity, Tag{"Brick"});
                    coordinator.AddComponent(entity, PhysicsBody{
                    coordinator.m_PhysicsManagerSystem->CreateB2Body(entity, STATIC_SOLID)});
                }
                
                m_BrickEntities.push_back(entity);
                m_B2BEntities.push_back(entity);
            }
        }
    }
}

void ECSFactory::CreateGroundEntity() 
{
    Tilemap& tilemap = coordinator.m_Tilemap;

    CleanGroundEntities();
    Entity ground;
    ground = coordinator.CreateEntity();
    coordinator.AddComponent(ground, Transform{Vec2D(
                    0.0f, (tilemap.m_MapY-1) * tilemap.m_TileHeight * tilemap.m_ScaleFactor)});
    coordinator.AddComponent(ground, Tag{"Ground"});
    m_GroundEntities.push_back(ground);
    coordinator.AddComponent(ground, PhysicsBody{
                coordinator.m_PhysicsManagerSystem->CreateB2Body(ground, STATIC_SOLID)});
}

void ECSFactory::CreateTestEntityFromJSON()
{
    Entity testEntity;
    if (FileReader::GetInstance().LoadJSONFile(COMPONENT_CONFIG)){
        testEntity = coordinator.CreateEntity();

        auto config = FileReader::GetInstance().GetConfig(COMPONENT_CONFIG);
        auto testEntityConfig = config["TestEntity"];

        coordinator.AddComponent(testEntity, Script{
            testEntityConfig["scriptName"]
        });
        coordinator.AddComponent(testEntity, Name{
            testEntityConfig["name"]
        });
        coordinator.AddComponent(testEntity, Transform{
            Vec2D(testEntityConfig["transformX"], testEntityConfig["transformY"])
        });
        coordinator.AddComponent(testEntity, InPut{});
        coordinator.AddComponent(testEntity, Sprite{
            coordinator.m_SpriteSystem->CreateSprite(
                testEntityConfig["filePath"], 
                coordinator.m_SDLGraphicsSystem->GetSDLRenderer(),
                testEntityConfig["sizeX"], 
                testEntityConfig["sizeY"], 
                testEntityConfig["transformX"],
                testEntityConfig["transformY"],
                testEntityConfig["frame"],
                testEntityConfig["renderSizeX"],
                testEntityConfig["renderSizeY"]
            )
        });

        coordinator.AddComponent(testEntity, Renderer{RENDER_ORDER::OBJECT});
        coordinator.m_RenderSystem->AddEntity(testEntity);
    }
}

void ECSFactory::CreateTestEntitySecondFromJSON()
{
    Entity testEntity2;
    if (FileReader::GetInstance().LoadJSONFile(COMPONENT_CONFIG)){
        testEntity2 = coordinator.CreateEntity();

        auto config = FileReader::GetInstance().GetConfig(COMPONENT_CONFIG);
        auto testEntityConfig = config["TestEntity2"];

        coordinator.AddComponent(testEntity2, Script{
            testEntityConfig["scriptName"]
        });
        coordinator.AddComponent(testEntity2, Name{
            testEntityConfig["name"]
        });
        coordinator.AddComponent(testEntity2, Transform{
            Vec2D(testEntityConfig["transformX"], testEntityConfig["transformY"])
        });
        coordinator.AddComponent(testEntity2, InPut{});
        coordinator.AddComponent(testEntity2, Sprite{
            coordinator.m_SpriteSystem->CreateSprite(
                testEntityConfig["filePath"], 
                coordinator.m_SDLGraphicsSystem->GetSDLRenderer(),
                testEntityConfig["sizeX"], 
                testEntityConfig["sizeY"], 
                testEntityConfig["transformX"],
                testEntityConfig["transformY"],
                testEntityConfig["frame"],
                testEntityConfig["renderSizeX"],
                testEntityConfig["renderSizeY"]
            )
        });

        coordinator.AddComponent(testEntity2, Renderer{RENDER_ORDER::OBJECT});
        coordinator.m_RenderSystem->AddEntity(testEntity2);
    }
}

void ECSFactory::CreatePlayerFromJSON()
{
    Entity player;
    coordinator.m_PlayerEntity = player;
    if (FileReader::GetInstance().LoadJSONFile(COMPONENT_CONFIG)){
        player = coordinator.CreateEntity();

        auto config = FileReader::GetInstance().GetConfig(COMPONENT_CONFIG);
        auto playerConfig = config["Player"];

        coordinator.AddComponent(player, InPut{});
        coordinator.AddComponent(player, Player{});
        coordinator.AddComponent(player, Transform{
            Vec2D(playerConfig["posX"], playerConfig["posY"])
        });
        coordinator.AddComponent(player, Sprite{
            coordinator.m_SpriteSystem->CreateSprite(
                playerConfig["filePath"], 
                coordinator.m_SDLGraphicsSystem->GetSDLRenderer(),
                playerConfig["sizeX"], 
                playerConfig["sizeY"], 
                playerConfig["posX"], 
                playerConfig["posY"], 
                playerConfig["frame"],
                playerConfig["renderSizeX"],
                playerConfig["renderSizeY"]
            )
        });
        coordinator.AddComponent(player, Script{
            playerConfig["scriptName"]
        });
        // coordinator.AddComponent(player, Camera{});

        coordinator.AddComponent(player, Renderer{RENDER_ORDER::OBJECT});
        coordinator.m_RenderSystem->AddEntity(player);
        // coordinator.AddComponent(player, PhysicsBody{
        // coordinator.m_PhysicsManagerSystem->CreateB2Body(player, DYNAMIC_SOLID)
        // });
    }
}

void ECSFactory::CreateTilemapFromJSON()
{
    Entity tilemap;
    vector<int> tempList;

    if (FileReader::GetInstance().LoadTilemapText(TILEMAP_TXT)
        && FileReader::GetInstance().LoadJSONFile(COMPONENT_CONFIG))
    {
        tilemap = coordinator.CreateEntity();
        
        auto config = FileReader::GetInstance().GetConfig(COMPONENT_CONFIG);
        auto tilemapConfig = config["Tilemap"];

        auto tilemapData = FileReader::GetInstance().GetTilemapData();
        
        coordinator.AddComponent(tilemap, Tilemap{
            coordinator.m_TilemapSystem->CreateTileMap(
                tilemapConfig["filePath"],
                tilemapConfig["rows"],
                tilemapConfig["cols"],
                tilemapConfig["tileWidth"],
                tilemapConfig["tileHeight"],
                tilemapConfig["mapX"],
                tilemapConfig["mapY"],
                tilemapConfig["scaleFactor"],
                tilemapData,
                tilemapConfig["gap"]
            )
        });

        coordinator.AddComponent(tilemap, InPut{});

        coordinator.AddComponent(tilemap, Renderer{RENDER_ORDER::BACKGROUND});
        coordinator.m_RenderSystem->AddEntity(tilemap);
    }
}

void ECSFactory::AddEntity(ENTITY_TYPE entity_type)
{
    Enemy enemy{100, 100};
    switch (entity_type)
    {
    case ENTITY_TYPE::ENEMY:
        // this->AddEnemyToJSON(enemy);
        break;
    
    default:
        break;
    }
}

// TODO: 读取多个，并生成出来；看懂tilemap，试试能不能做出生成tilemap;
void ECSFactory::AddEnemyToJSON(Enemy enemyData)
{
    // if (FileReader::GetInstance().LoadJSONFile(COMPONENT_CONFIG))
    // {
    //     // create entity
    //     Entity enemy;
    //     enemy = coordinator.CreateEntity();

    //     // create a new enemy
    //     json newEnemyConfig = {
    //         {"health", enemyData.health},
    //         {"speed" , enemyData.speed}
    //     };

    //     // get config object
    //     auto currentConfig = FileReader::GetInstance().GetConfig();
    //     currentConfig["Enemy"].push_back(newEnemyConfig);

    //     // add entities to the enemy
    //     coordinator.AddComponent(enemy, enemyData);
    //     // TODO: Create sprite here
    //     coordinator.AddComponent(enemy, Sprite{
    //         coordinator.m_SpriteSystem->CreateSprite(
    //             "./images/sprite.bmp",
    //             coordinator.m_SDLGraphicsSystem->GetSDLRenderer(),
    //             10,
    //             10,
    //             6
    //         )
    //     });
    //     coordinator.AddComponent(enemy, Renderer{RENDER_ORDER::OBJECT});
    //     coordinator.m_RenderSystem->AddEntity(enemy);

    //     FileReader::GetInstance().SaveJSONFile(currentConfig, COMPONENT_CONFIG);

    // }
}

void ECSFactory::CreateMenuUI()
{
    if (!this->m_IsUIConfigFileExist)
    {
        cout << "Cannot open: " << UI_CONFIG << endl;
        return;
    }
    else
    {
        Entity Img_Menu;

        // background
        CreateUIElement(Img_Menu, "Img_Menu", m_ImageConfig, GAME_STATE::MENU, UIType::MENU_IMAGE, RENDER_ORDER::_UI_);
    }
}

void ECSFactory::CreateEditorUI()
{
    if(!m_IsUIConfigFileExist){
        cout << "Cannot open: " << UI_CONFIG << endl;
        return;
    }
    else{

        Entity btn_AddTile;
        Entity btn_RemoveTile;

        // Create the button based on you want to render the button(ex. EDITOR, GAME, MENU)
        // CreateUIElement(btn_AddTile, "Btn_AddTile", m_ButtonConfig, GAME_STATE::EDITOR, UIType::EDITOR_BUTTON, RENDER_ORDER::_UI_);
        // CreateUIElement(btn_AddTile, "Btn_RemoveTile", m_ButtonConfig, GAME_STATE::EDITOR, UIType::EDITOR_BUTTON, RENDER_ORDER::_UI_);
    }
}

void ECSFactory::CreateUIElement(Entity entity, const string& btnName, const json& buttonConfig, GAME_STATE state, UIType uiType, RENDER_ORDER renderOrder)
{
    entity = coordinator.CreateEntity();

    switch (state)
    {
    case GAME_STATE::EDITOR:
        coordinator.AddComponent(entity, Editor{});
        coordinator.AddComponent(entity, UIComponent{uiType});
        break;
    case GAME_STATE::GAME:
        coordinator.AddComponent(entity, UIComponent{uiType});
        break;
    case GAME_STATE::MENU:
        coordinator.AddComponent(entity, UIComponent{uiType});
        break;
    default:
        break;
    }

    coordinator.AddComponent(entity, Sprite{
        coordinator.m_SpriteSystem->CreateSprite(
                buttonConfig[btnName]["filePath"],
                coordinator.m_SDLGraphicsSystem->GetSDLRenderer(),
                buttonConfig[btnName]["size"]["w"],
                buttonConfig[btnName]["size"]["h"],
                buttonConfig[btnName]["position"]["x"], 
                buttonConfig[btnName]["position"]["y"], 
                0
            )
    });

    coordinator.AddComponent(entity, Renderer{renderOrder});
    coordinator.m_RenderSystem->AddEntity(entity);
}

void ECSFactory::RegisterCommonSystem()
{
    if (coordinator.m_TilemapSystem == nullptr)
    {
        coordinator.m_TilemapSystem = coordinator.RegisterSystem<TilemapSystem>();
        {
            Signature signature;
            signature.set(coordinator.GetComponentTypeUniqueIndex<Tilemap>());

            coordinator.SetSignature<TilemapSystem>(signature);
        }
        coordinator.m_TilemapSystem->Init();
    }

    if (coordinator.m_LevelEditorSystem == nullptr)
    {
        coordinator.m_LevelEditorSystem = coordinator.RegisterSystem<LevelEditorSystem>();
        {
            Signature signature;
            signature.set(coordinator.GetComponentTypeUniqueIndex<Editor>());

            coordinator.SetSignature<LevelEditorSystem>(signature);
        }
        coordinator.m_LevelEditorSystem->Init();
    }

    if (coordinator.m_CameraSystem == nullptr)
    {
        coordinator.m_CameraSystem = coordinator.RegisterSystem<CameraSystem>();
        {
            Signature signature;
            signature.set(coordinator.GetComponentTypeUniqueIndex<Camera>());

            coordinator.SetSignature<CameraSystem>(signature);
        }
        coordinator.m_CameraSystem->Init();
    }
}

void ECSFactory::RegisterMenuSystem()
{
    coordinator.m_SDLGraphicsSystem = coordinator.RegisterSystem<SDLGraphicsSystem>();
    {
        coordinator.m_SDLGraphicsSystem->SetSDLWindow(WINDOW_WIDTH, WINDOW_HIEGHT);
    }
    coordinator.m_SDLGraphicsSystem->Init();

    coordinator.m_SpriteSystem = coordinator.RegisterSystem<SpriteSystem>();
    {
        Signature signature;
        signature.set(coordinator.GetComponentTypeUniqueIndex<Sprite>());

        coordinator.SetSignature<SpriteSystem>(signature);
    }
    coordinator.m_SpriteSystem->Init();

    coordinator.m_RenderSystem = coordinator.RegisterSystem<RenderSystem>();
    {
        Signature signature;
        signature.set(coordinator.GetComponentTypeUniqueIndex<Renderer>());

        coordinator.SetSignature<RenderSystem>(signature);
    }
    coordinator.m_RenderSystem->Init();

    coordinator.m_UISystem = coordinator.RegisterSystem<UISystem>();
    {
        Signature signature;
        signature.set(coordinator.GetComponentTypeUniqueIndex<UIComponent>());

        coordinator.SetSignature<UISystem>(signature);
    }
    coordinator.m_UISystem->Init();

    if (coordinator.m_InputSystem == nullptr)
    {
        
    }
    coordinator.m_InputSystem = coordinator.RegisterSystem<InputSystem>();
    {
        Signature signature;
        signature.set(coordinator.GetComponentTypeUniqueIndex<InPut>());

        coordinator.SetSignature<InputSystem>(signature);
    }
    coordinator.m_InputSystem->Init();
}

void ECSFactory::RegisterGameSystem()
{
    coordinator.m_PhysicsManagerSystem = coordinator.RegisterSystem<PhysicsManagerSystem>();
    {
        Signature signature;
        signature.set(coordinator.GetComponentTypeUniqueIndex<PhysicsBody>());

        coordinator.SetSignature<PhysicsManagerSystem>(signature);
    }
    coordinator.m_PhysicsManagerSystem->Init();
    coordinator.m_PhysicsManagerSystem->SetGravity(0, 98);

    coordinator.m_ControllerSystem = coordinator.RegisterSystem<ControllerSystem>();
    {
        Signature signature;

        // here, set a unique signature based on the component attached
        coordinator.SetSignature<ControllerSystem>(signature);
    }
    coordinator.m_ControllerSystem->Init();

    coordinator.m_ScriptingSystem = coordinator.RegisterSystem<ScriptingSystem>();
    {
        Signature signature;

        // set a subsystem's unique Signature
        signature.set(coordinator.GetComponentTypeUniqueIndex<Script>());

        // here, set a unique signature based on the component attached
        coordinator.SetSignature<ScriptingSystem>(signature);
    }
    coordinator.m_ScriptingSystem->Init();
}

void ECSFactory::RegisterMenuEntity()
{
    CreateMenuUI();
}

void ECSFactory::RegisterCommonEntity()
{
    CreateTilemapFromJSON();

    // CreateEditorUI();
}

void ECSFactory::CleanBrickTransform()
{
    m_BrickTransform.clear();
    if (m_BrickTransform.size() > 0)
    {
        for (auto &entities : m_BrickTransform)
        {
            if (coordinator.HashComponent<Transform>(entities))
            {
                coordinator.getEntityManager().DestroyEntity(entities);
            }
        }
    }
}

void ECSFactory::CleanBrickEntities()
{
    m_BrickEntities.clear();
    cout << "CleanBrickEntities with size: " << m_BrickEntities.size();
    if (m_BrickEntities.size() > 0)
    {
        for (auto &entities : m_BrickEntities)
        {
            if (coordinator.HashComponent<Tag>(entities))
            {
                auto &tag = coordinator.GetComponent<Tag>(entities);
                if (tag.theTag == "Brick")
                {
                    coordinator.getEntityManager().DestroyEntity(entities);
                }
            }
        }
    }
}

void ECSFactory::CleanGroundEntities()
{
    if (m_GroundEntities.size() > 0)
    {
        for (auto &entities : m_GroundEntities)
        {
            coordinator.getEntityManager().DestroyEntity(entities);
        }
    }
}

void ECSFactory::CleanBrickB2Body()
{
    m_B2BEntities.clear();
    cout << "CleanBrickB2Body with size: " << m_B2BEntities.size() << endl;
    if (m_B2BEntities.size() > 0)
    {
        for (auto &entities : m_B2BEntities)
        {
            if (coordinator.HashComponent<PhysicsBody>(entities))
            {
                coordinator.m_PhysicsManagerSystem->RemoveB2Body(entities);
                coordinator.getEntityManager().DestroyEntity(entities);
            }
        }
    }
}

void ECSFactory::CreateEnemyFromJSON()
{
    if (FileReader::GetInstance().LoadJSONFile(COMPONENT_CONFIG))
    {
        // get config object
        auto config = FileReader::GetInstance().GetConfig(COMPONENT_CONFIG);
        auto enemyConfig = config["Enemy"];

        for(int i = 0; i < enemyConfig.size(); i++){
            // create entity
            Entity enemy;
            enemy = coordinator.CreateEntity();

            // 添加Enemy组件
            coordinator.AddComponent(enemy, Enemy{
                enemyConfig[i]["health"],
                enemyConfig[i]["speed"]
            });

            // 添加Sprite组件
            coordinator.AddComponent(enemy, Sprite{
                // 使用你的创建Sprite的方法
                coordinator.m_SpriteSystem->CreateSprite(
                    enemyConfig[i]["filePath"],
                    coordinator.m_SDLGraphicsSystem->GetSDLRenderer(),
                    enemyConfig[i]["sizeX"],
                    enemyConfig[i]["sizeY"],
                    enemyConfig[i]["posX"], 
                    enemyConfig[i]["posY"], 
                    enemyConfig[i]["frame"]
                )
            });
            
            // 添加Input组件，因为鼠标要检测是否碰到了enemy，需要在inputSystem中用到这个entity
            coordinator.AddComponent(enemy, InPut{});

            coordinator.AddComponent(enemy, Renderer{RENDER_ORDER::OBJECT});
            coordinator.m_RenderSystem->AddEntity(enemy);
        }
    }
}

void ECSFactory::CreateCamera()
{
    Entity camera;
    camera = coordinator.CreateEntity();

    if(m_IsConfigFileExist){
        coordinator.AddComponent(camera, Camera{
                coordinator.m_CameraSystem->CreateCamera(
                    m_CameraConfig["cameraX"],
                    m_CameraConfig["cameraY"],
                    m_CameraConfig["viewportWidth"],
                    m_CameraConfig["viewportHeight"],
                    m_CameraConfig["worldWidth"],
                    m_CameraConfig["worldHeight"],
                    m_CameraConfig["edge"]
                )
            });
    }
}
