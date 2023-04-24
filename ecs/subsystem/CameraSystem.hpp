#pragma once

#include "System.hpp"
#include "Event.hpp"
#include "Camera.hpp"

class CameraSystem : public ISystem
{
public:
	void Init() override;

	void Update() override;

    void Clear() override;

    void Follow(int x, int y);

    void MoveCamera(int x, int y);

    void EnterState(const GAME_STATE& state);

    /// @brief Create a camera object
    /// @param cameraX camera position x
    /// @param cameraY camera position y
    /// @param viewportWidth camera viewport width
    /// @param viewportHeight camera viewport height
    /// @param worldWidth world width
    /// @param worldHeight world height
    /// @param edge viewport edge
    /// @return 
    Camera CreateCamera(float cameraX, float cameraY, float viewportWidth, float viewportHeight,
                      float worldWidth, float worldHeight,float edge);

private:
    // below are functions that may be called when the event is triggered

    void OnKeyboardClicked_A(const Event &event);

    void OnKeyboardClicked_D(const Event &event);

    void OnKeyboardClicked_W(const Event &event);

    void OnKeyboardClicked_S(const Event &event);

    void OnPlayerMovePosition(const Event &event);

    void OnTilemapChanged(const Event &event);

    /// @brief dispatch camera position [EVENT_ON_CAMERA_MOVE]
    /// @param x camera posX
    /// @param y camera posY
    void DispatchCameraMovementPosition(int x, int y);

    /// @brief stores the camera object when CreateCamera is called
    Camera      m_Camera;

    /// @brief the viewport of the amera
    SDL_Rect    m_CameraInnerViewport;

    /// @brief camera move speed
    int         m_CameraSpeed;

    /// @brief map max width
    int         m_MaxMapWidth;

    /// @brief player postion X
    int         m_PlayerMoveX;
    /// @brief player position y
    int         m_PlayerMoveY;

    /// @brief check if we have the player entity, if so, follow the player's position
    bool        m_HasPlayerEntity;
};