#pragma once
#include <string>
#include "level_data.h"
#include "character_data.h"
#include "twoc/binary_reader.h"

namespace Twoc {

// provides access o HData, LData and ModelLists
class Executable
{
protected:
    std::vector<LevelData> m_level_data;
    std::vector<CharacterData> m_character_data;
    std::vector<std::uint8_t> m_hub_data;

public:
    Executable(std::vector<LevelData> level_data, std::vector<std::uint8_t> hub_data, std::vector<CharacterData> character_data);

    std::vector<LevelData> const& level_data() const { return m_level_data; }
    std::vector<std::uint8_t> hub_data() const { return m_hub_data; }
    std::vector<CharacterData> const& character_data() const { return m_character_data; }

    CharacterData character_data(int index) const { return m_character_data[index]; }
    LevelData const& level_data(int index) const { return m_level_data[index]; }
    std::uint8_t hub_data(int index) const { return m_hub_data[index]; }
};

class XboxExecutable : public Executable
{
public:
    static XboxExecutable *from_reader(Twoc::BinaryReader& reader);
    XboxExecutable(std::vector<LevelData> level_data, std::vector<std::uint8_t> hub_data, std::vector<CharacterData> character_data);
};

class ElfExecutable : public Executable
{
public:
    static ElfExecutable *from_reader(Twoc::BinaryReader& reader);

    ElfExecutable(std::vector<LevelData> level_data, std::vector<std::uint8_t> hub_data, std::vector<CharacterData> character_data);
};

}

