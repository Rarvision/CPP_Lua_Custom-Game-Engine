#ifndef SDLGRAPHICSPROGRAM
#define SDLGRAPHICSPROGRAM

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "ECSCoordinator.hpp"
#include "ECSFactory.hpp"
#include "Common.hpp"

// managers
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "EventManager.hpp"


/**
 * This class sets up the main game engine
 */
class Engine{
public:
    /**
     * Constructor of Engine
     */
    Engine();
    /**
     * Destructor
     */
    ~Engine();
    /**
     * Input engine
     */
    void Input(bool *quit);
    /**
     * Per frame update
     */
    void Update();
    /**
     * Per frame render. Renders everything
     */
    void Render();
    /**
     * Main Game Loop that runs forever
     */
    void MainGameLoop();
    /**
     * Initialization and shutdown pattern
     * Explicitly call 'Start' to launch the engine
     */
    void Start();
    /**
     * Initialization and shutdown pattern
     * Explicitly call 'Shutdown' to terminate the engine
     */
    void Shutdown();
    
    /// @brief set engine frame rate
    void SetFrameRate();

    /// @brief init engine, the start enterance of the program
    void Init();


private:
    /// @brief a pointer that points to the ECSCoordinator
    unique_ptr<ECSCoordinator>      m_ECSCoordinator;
};
#endif
