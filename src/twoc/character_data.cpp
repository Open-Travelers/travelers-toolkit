#include "character_data.h"
#include "elf/file.h"

namespace Twoc
{

ITEM_DEFINE(RawCharacterData, TWOC_CDATA_ITEM)
ITEM_DEFINE(RawCharacterDataE3, TWOC_CDATA_E3_ITEM)

bool RawCharacterData::validate()
{
    return true;
}

bool RawCharacterDataE3::validate()
{
    return true;
}

CharacterData::CharacterData(RawCharacterData data, std::string path, std::string file, std::string name)
{
    Type = GameType::Retail;
    Path = path;
    File = file;
    Name = name;
    Radius = data.radius();

    Min = { data.min(0), data.min(1), data.min(2) };
    Max = { data.max(0), data.max(1), data.max(2) };
    Scale = data.scale();
    ShadowScale = data.shadow_scale();

    Unk1 = Unk2 = Unk3 = Unk4 = 0;
}

CharacterData::CharacterData(RawCharacterDataE3 data, std::string path, std::string file, std::string name)
{
    Type = GameType::E3;
    Path = path;
    File = file;
    Name = name;
    Radius = data.radius();
    Unk1 = data.unk1();
    Unk2 = data.unk2();
    Unk3 = data.unk3();
    Unk4 = data.unk4();
}


}
