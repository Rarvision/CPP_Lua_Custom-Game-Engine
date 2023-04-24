#include "FileReader.hpp"

using json = nlohmann::json;

bool FileReader::LoadJSONFile(const string& fileName){
    ifstream file(fileName);

    if(!file.is_open()){
        // Debug::PrintMessage("Failed to open JSON file");
        cout << "Failed to open JSON file: " << fileName << endl;
        return false;
    }

    try
    {
        if(fileName == UI_CONFIG){
            file >> m_UIConfig;
        }
        if(fileName == COMPONENT_CONFIG){
            file >> m_ComponentConfig;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    file.close();
    
    return true;
}

void FileReader::SaveJSONFile(const json currentJSONConfig, const string& jsonFileName)
{
    ofstream jsonFile(jsonFileName);
    
    jsonFile << currentJSONConfig.dump(4);

    jsonFile.close();
}

const json& FileReader::GetConfig(const string& fileName) const {
    if(fileName == COMPONENT_CONFIG){
        return m_ComponentConfig;
    }
    if(fileName == UI_CONFIG){
        return m_UIConfig;
    }

    return NULL;
}

bool FileReader::LoadTilemapText(const string &fileName)
{
    m_TilemapData.clear();
    ifstream file(fileName);
    if(!file){
        Debug::PrintMessage("Unable to open map file!");
        return false;
    }  

    // string line;

    // vector<vector<int>> tempTiles;

    // while(getline(file, line)){
    //     vector<int> row;
    //     istringstream iss(line);
    //     int tileType;

    //     while(iss >> tileType){
    //         row.push_back(tileType);
    //     }

    //     tempTiles.push_back(row);
    // }

    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        int tileType;
        while (iss >> tileType)
        {
            m_TilemapData.push_back(tileType);
        }
    }

    for (int y = 0; y < 17; y++)
    {
        for (int x = 0; x < 50; x++)
        {
            cout << m_TilemapData[y * 50 + x] << " ";
        }
        cout << endl;
    }

    // for (int i = 0; i < m_TilemapData.size(); i++)
    // {
    //     cout << m_TilemapData[i] << " ";
    // }

    file.close();

    return true;
}

void FileReader::SaveTilemapText(int rows, int cols, const vector<int> &mapData, const string &fileName)
{
    // 存储最最新的地图长度
    if (LoadJSONFile(COMPONENT_CONFIG)){
        // TODO: cannot save this value
        m_ComponentConfig["Tilemap"]["mapX"] = rows;
        SaveJSONFile(m_ComponentConfig, COMPONENT_CONFIG);
        cout << "Storing map width as: " << m_ComponentConfig["Tilemap"]["mapX"];
    }
    // std::cout << "rows: " << rows << ", cols: " << cols << std::endl;
    // for (int i = 0; i < cols; i++)
    // {
    //     for (int j = 0; j < rows; j++)
    //     {
    //          cout << mapData[i * rows + j] << " ";
    //     }
    //     cout << endl;
    // }
    ofstream out(fileName);

    // std::cout << "rows: " << rows << ", cols: " << cols << std::endl;
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            out << mapData[i * rows + j] << " ";
        }
        out << '\n';
    }

    out.close();
}

const vector<int> &FileReader::GetTilemapData() const
{
    // TODO: 在此处插入 return 语句
    return m_TilemapData;
}
