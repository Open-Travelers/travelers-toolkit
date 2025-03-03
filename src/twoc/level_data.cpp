#include "level_data.h"
#include "elf/file.h"

namespace Twoc
{

ITEM_DEFINE(RawLevelData, TWOC_LDATA_ITEM)
ITEM_DEFINE(RawLevelDataE3, TWOC_LDATA_E3_ITEM)

bool RawLevelData::validate()
{
    return true;
}

bool RawLevelDataE3::validate()
{
    return true;
}

LevelData::LevelData(RawLevelData data, std::string filepath, std::vector<std::string> names, std::vector<std::uint8_t> model_list)
{
    Type = GameType::Retail;
    Filepath = filepath;
    Names = names;
    ModelList = model_list;
    for (int i = 0; i < sizeof(Time)/sizeof(*Time); i++)
        Time[i] = data.time(i);
    for (int i = 0; i < sizeof(Music)/sizeof(*Music); i++)
        Music[i] = data.music(i);

    Hub = data.hub();
    Flags = data.flags();
    Character = data.character();
    Vehicle = data.vehicle();
    FarClippingPlane = data.far_plane();
    Start = { data.start(0), data.start(1), data.start(2) };
    Bonus = { data.bonus(0), data.bonus(1), data.bonus(2) };

    FogNear = data.fog_near();
    FogFar = data.fog_far();
    for (int i = 0; i < sizeof(FogColor)/sizeof(*FogColor); i++)
        FogColor[i] = data.fog(i);
    for (int i = 0; i < sizeof(HazeColor)/sizeof(*HazeColor); i++)
        HazeColor[i] = data.haze(i);
    TotalSize = data.total_size();

    ClockPosition = glm::vec3 {0};
    GemPosition = glm::vec3 {0};
    CrystalPosition = glm::vec3 {0};
    DirLight1 = glm::vec3 {0};
    DirLight2 = glm::vec3 {0};

    GradientStart = 0;
    GradientEnd = 0;
    Unk1 = 0;
    Unk2 = glm::vec3 {0};
    Unk3 = glm::vec3 {0};

}

LevelData::LevelData(RawLevelDataE3 data, std::string filepath, std::vector<std::string> names, std::vector<std::uint8_t> model_list)
{
    Type = GameType::E3;
    Filepath = filepath;
    Names = names;
    ModelList = model_list;
    for (int i = 0; i < sizeof(Time)/sizeof(*Time); i++)
        Time[i] = 0;
    for (int i = 0; i < sizeof(Music)/sizeof(*Music); i++)
        Music[i] = 0;

    Hub = data.hub();
    Flags = data.flags();
    Character = data.character();
    Vehicle = data.vehicle();
    FarClippingPlane = data.far_plane();
    Start = glm::vec3 { 0 };
    Bonus = { data.bonus(0), data.bonus(1), data.bonus(2) };

    FogNear = data.fog_near();
    FogFar = data.fog_far();
    for (int i = 0; i < sizeof(FogColor)/sizeof(*FogColor); i++)
        FogColor[i] = data.fog(i);
    for (int i = 0; i < sizeof(HazeColor)/sizeof(*HazeColor); i++)
        HazeColor[i] = data.haze(i);
    TotalSize = 0;

    ClockPosition = { data.clock_position(0), data.clock_position(1), data.clock_position(2) };
    GemPosition = { data.crate_gem_position(0), data.crate_gem_position(1), data.crate_gem_position(2) };
    CrystalPosition = { data.crystal_position(0), data.crystal_position(1), data.crystal_position(2) };
    DirLight1 = { data.directional_light_1(0), data.directional_light_1(1), data.directional_light_1(2) };
    DirLight2 = { data.directional_light_2(0), data.directional_light_2(1), data.directional_light_2(2) };

    GradientStart = data.gradient_start();
    GradientEnd = data.gradient_end();
    Unk1 = data.unk1();
    Unk2 = { data.unk2(0), data.unk2(1), data.unk2(2) };
    Unk3 = { data.unk3(0), data.unk3(1), data.unk3(2) };
}

}
