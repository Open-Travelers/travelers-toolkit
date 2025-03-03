#pragma once
#include <cstdint>
#include <string>
#include "glm/vec3.hpp"
#include "binary_reader.h"
#include "reader_item.h"
#include "level_data.h"

namespace Twoc
{

#define TWOC_CDATA_ITEM(i, a) \
    i(uint32_t, p_path) \
    i(uint32_t, p_file) \
    i(uint32_t, p_name) \
    i(uint32_t, p_animation_list) \
    i(float, radius) \
    a(float, min, 3) \
    a(float, max, 3) \
    i(float, scale) \
    i(float, shadow_scale)

#define TWOC_CDATA_E3_ITEM(i, a) \
    i(uint32_t, p_path) \
    i(uint32_t, p_file) \
    i(uint32_t, p_name) \
    i(uint32_t, p_animation_list) \
    i(float, radius) \
    i(float, unk1) \
    i(float, unk2) \
    i(float, unk3) \
    i(float, unk4)


ITEM_DECLARE(RawCharacterData, TWOC_CDATA_ITEM)
ITEM_DECLARE(RawCharacterDataE3, TWOC_CDATA_E3_ITEM)

struct CharacterData {
    GameType Type;

    std::string Path;
    std::string File;
    std::string Name;
    float Radius;

    glm::vec3 Min;
    glm::vec3 Max;
    float Scale;
    float ShadowScale;

    float Unk1;
    float Unk2;
    float Unk3;
    float Unk4;

    CharacterData(RawCharacterData data,   std::string path, std::string file, std::string name);
    CharacterData(RawCharacterDataE3 data, std::string path, std::string file, std::string name);
};

}
