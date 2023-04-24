#pragma once
// store all variables/types here
#include <cstdint>
#include <bitset>
#include "json.hpp"

using namespace std;

/// @brief JSON
using json = nlohmann::json;

/// @brief Each Entity has a unique ID
using Entity = size_t;

/// @brief ComponentType is used for set a unique ID for a component
using ComponentType = std::uint8_t;

/// @brief max component size
const ComponentType MAX_COMPONENTS = 32;

/// @brief max entity size
const int MAX_EntityCount = 1000;

/// @brief Signature is used to recognize what the component is
using Signature = bitset<MAX_COMPONENTS>;

/// @brief Event Type
enum EVENT{
    EVENT_ON_KEYBOARD_CLICKED_A,
    EVENT_ON_KEYBOARD_CLICKED_D,
    EVENT_ON_KEYBOARD_CLICKED_W,
    EVENT_ON_KEYBOARD_CLICKED_S,
    EVENT_ON_KEYBOARD_CLICKED_O,
    EVENT_ON_KEYBOARD_CLICKED_I,
    EVENT_ON_KEYBOARD_CLICKED_U,
    EVENT_ON_KEYBOARD_CLICKED_SPACE,
    EVENT_ON_MOUSE_CLICKED_LEFT,
    EVENT_ON_MOUSE_CLICKED_RIGHT,
    EVENT_ON_LEVEL_DATA_LOADED,
    EVENT_ON_TILEMAP_CHANGED,
    EVENT_ON_CAMERA_MOVE,
    EVENT_ON_CAMERA_MOVE_LEFT,
    EVENT_ON_CAMERA_MOVE_RIGHT,
    EVENT_ON_PLAYER_MOVE
};
// constexpr

// SDL Render Type
enum SDLType{
    Surface,
    Texture
};

/// @brief Window width
constexpr int WINDOW_WIDTH = 1920;
/// @brief Window height
constexpr int WINDOW_HIEGHT = 1080;
/// @brief Editor height
constexpr int EDITOR_WIDTH = 640;
constexpr int FPS = 60;
constexpr float Delta = 1000 / FPS;
constexpr int FRAME = 60;

/// @brief All paths for resources like sprite, json, tilemap text
constexpr const char* COMPONENT_CONFIG = "./config/ComponentConfig.json";
constexpr const char* TILEMAP_BMP = "./Resources/images/Tilemap2.bmp";
constexpr const char* ENEMY_SPRITE = "./images/sprite.bmp";
constexpr const char* TILEMAP_TXT = "./config/Tilemap_Data.txt";
constexpr const char* UI_CONFIG = "./config/UIConfig.json";
constexpr const char* AUDIO_COIN = "./Resources/Audio/smb_coin.wav";
constexpr const char* AUDIO_GAMEOVER = "./Resources/Audio/smb_gameover.wav";
constexpr const char* AUDIO_JUMP = "./Resources/Audio/smb_gameover.wav";
constexpr const char* AUDIO_DIE = "./Resources/Audio/smb_gameover.wav";
constexpr const char* AUDIO_BGM = "./Resources/Audio/smb_bgm.wav";

// Render Order: the bigger number renders first
enum RENDER_ORDER{
    BACKGROUND,
    OBJECT,
    _UI_
};

// Entity Type
enum ENTITY_TYPE{
    ENEMY
};

// Game State
enum GAME_STATE{
    MENU,
    EDITOR,
    GAME
};

// Physics Body Type
enum BodyType {
    DYNAMIC_SOLID,
    STATIC_SOLID,
    STATIC_SENSOR
};

// Tile Dimensions
const int TILE_W = 64;
const int TILE_H = 64;