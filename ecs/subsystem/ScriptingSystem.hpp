#pragma once
#include <string>
#include <set>

using namespace std;

class ScriptingSystem : public ISystem {
public:
	void Init() override;

    void BindFunctions();

	void Update() override;

    void Clear();

private:
    void Load(string);
    void Execute(string);
    
    set<Entity> init_set;
};
