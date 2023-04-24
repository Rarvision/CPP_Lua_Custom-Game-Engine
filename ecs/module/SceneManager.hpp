#pragma once

#include "Common.hpp"
#include <memory>

class ECSFactory;

class SceneManager{
public:
    SceneManager();

    void EnterState(GAME_STATE newState);

    /// @brief init into a state
    /// @param state 
    void InitState(GAME_STATE state);

    /// @brief release resource in this state
    /// @param state 
    void CleanupState(GAME_STATE state);

    /// @brief init menu
    void InitMenu();

    /// @brief init editor
    void InitEditor();

    /// @brief init game
    void InitGame();

    /// @brief clean menu state
    void CleanupMenu();

    /// @brief clean editor state
    void CleanupEditor();

    /// @brief clean game state
    void CleanupGame();

private:

    /// @brief record current state
    GAME_STATE                  m_CurrentState;

    /// @brief a shared_ptr that points to the ECSFactory
    shared_ptr<ECSFactory>      m_ECSFactory;
};