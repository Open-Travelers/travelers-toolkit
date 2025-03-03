
#include "executable.h"

namespace Twoc
{

Executable::Executable(std::vector<LevelData> level_data, std::vector<std::uint8_t> hub_data, std::vector<CharacterData> character_data) : m_level_data(level_data), m_hub_data(hub_data), m_character_data(character_data)
{

}

XboxExecutable::XboxExecutable(std::vector<LevelData> level_data, std::vector<std::uint8_t> hub_data, std::vector<CharacterData> character_data) : Executable(level_data, hub_data, character_data)
{

}

XboxExecutable *XboxExecutable::from_reader(Twoc::BinaryReader& reader)
{
    return nullptr;
}

}
