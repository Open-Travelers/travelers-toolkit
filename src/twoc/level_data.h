#pragma once
#include <cstdint>
#include <string>
#include "glm/vec3.hpp"
#include "binary_reader.h"
#include "reader_item.h"

namespace Twoc
{

enum LevelFlags {
    UseOldTerrain = 1 << 3,
    UsePtlEffects = 1 << 4,
    SomeLevelHex25Flag = 0x800
};

#define TWOC_LDATA_ITEM(i, a) \
    i(uint32_t, p_filepath) \
    i(uint32_t, p_modellist) \
    i(uint32_t, p_chase) \
    a(uint32_t, time, 3) \
    a(int16_t, music, 2) \
    i(uint32_t, p_sfx) \
    i(int16_t, sfx_count) \
    i(uint8_t, __padding) \
    i(int8_t, hub) \
    i(uint16_t, flags) \
    i(int16_t, character) \
    i(int16_t, vehicle) \
    i(uint16_t, far_plane) \
    a(float, start, 3) \
    a(float, bonus, 3) \
    i(float, fog_near) \
    i(float, fog_far) \
    a(uint8_t, fog, 4) \
    a(uint8_t, haze, 4) \
    i(int32_t, total_size)

#define TWOC_LDATA_E3_ITEM(i, a) \
    i(uint32_t, p_filepath) \
    i(uint32_t, unk1) \
    i(uint32_t, p_modellist) \
    i(uint32_t, p_chase) \
    i(uint8_t, __padding) \
    i(int8_t, hub) \
    i(uint16_t, flags) \
    a(float, unk2, 3) \
    i(int8_t, character) \
    i(int8_t, vehicle) \
    i(uint16_t, far_plane) \
    a(float, clock_position, 3) \
    a(float, crate_gem_position, 3) \
    a(float, crystal_position, 3) \
    a(float, bonus, 3) \
    a(float, directional_light_1, 3) \
    a(float, directional_light_2, 3) \
    i(uint32_t, gradient_start) \
    i(uint32_t, gradient_end) \
    i(uint16_t, fog_near) \
    i(uint16_t, fog_far) \
    a(uint8_t, fog, 4) \
    a(uint8_t, haze, 4) \
    a(float, unk3, 3)

ITEM_DECLARE(RawLevelData, TWOC_LDATA_ITEM)
ITEM_DECLARE(RawLevelDataE3, TWOC_LDATA_E3_ITEM)

enum class GameType
{
    Retail,
    E3
};

struct LevelData {
    GameType Type;
    std::vector<std::string> Names;
    std::string Filepath;
    std::vector<std::uint8_t> ModelList;
    std::uint32_t Time[3];
    std::int16_t Music[2];
    std::int8_t Hub;
    std::uint16_t Flags;
    std::int16_t Character;
    std::int16_t Vehicle;
    std::uint16_t FarClippingPlane;
    glm::vec3 Start;
    glm::vec3 Bonus;
    float FogNear;
    float FogFar;
    std::uint8_t FogColor[4];
    std::uint8_t HazeColor[4];
    std::int32_t TotalSize;

    glm::vec3 ClockPosition;
    glm::vec3 GemPosition;
    glm::vec3 CrystalPosition;
    glm::vec3 DirLight1;
    glm::vec3 DirLight2;

    uint32_t GradientStart;
    uint32_t GradientEnd;
    uint32_t Unk1;
    glm::vec3 Unk2;
    glm::vec3 Unk3;

    LevelData(RawLevelData data, std::string filepath, std::vector<std::string> names, std::vector<std::uint8_t> model_list);
    LevelData(RawLevelDataE3 data, std::string filepath, std::vector<std::string> names, std::vector<std::uint8_t> model_list);
};

}
