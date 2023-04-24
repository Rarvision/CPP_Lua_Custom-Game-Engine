#pragma once

#include "json.hpp"
#include "Singleton.hpp"
#include "Debug.hpp"
#include "Common.hpp"

#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

using json = nlohmann::json;

class FileReader: public Singleton<FileReader>{
public:
    FileReader() = default;

    ~FileReader() = default;

    bool LoadJSONFile(const string& fileName);

    void SaveJSONFile(const json, const string& jsonFileName);

    const json& GetConfig(const string& fileName) const;

    bool LoadTilemapText(const string& fileName);

    void SaveTilemapText(int rows, int cols, const vector<int>& mapData, const string &fileName);
    
    const vector<int>& GetTilemapData() const;
private:
    // if you want to load a new json file, make sure you create a new json object to store it.
    json                m_UIConfig;
    json                m_ComponentConfig;
    vector<int>         m_TilemapData;
};