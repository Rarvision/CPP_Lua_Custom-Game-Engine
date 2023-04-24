#include "Engine.hpp" // The main engine
#include "Common.hpp"
#include "FileReader.hpp"

#include <fstream>

Engine engine;

ECSCoordinator coordinator;

int main(int argc, char** argv){

    engine.Init();

    engine.MainGameLoop();

    engine.Shutdown();

	return 0;
}
