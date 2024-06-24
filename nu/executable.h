
#ifndef EXECUTABLE_H
#define EXECUTABLE_H
#include <vector>
#include <string>

namespace Nu
{

class LevelData {
protected:
    std::string m_name;
    std::vector<int> m_model_list;


};

class Executable
{
public:
protected:
    std::vector<LevelData> m_level_data;
    int m_hub_data[5*6];
};

}
#endif