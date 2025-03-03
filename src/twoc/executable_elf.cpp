#include <iostream>
#include <cstring>
#include <cstdint>

#include "executable.h"
#include "twoc/binary_reader.h"
#include "twoc/elf/file.h"
#include "twoc/elf/symbol.h"
#include "twoc/reader_item.h"
#include "twoc/level_data.h"
namespace Twoc
{

ElfExecutable::ElfExecutable(std::vector<LevelData> level_data, std::vector<std::uint8_t> hub_data, std::vector<CharacterData> character_data) : Executable(level_data, hub_data, character_data)
{

}

ElfExecutable *ElfExecutable::from_reader(Twoc::BinaryReader& reader)
{
    Elf::File file;
    if (!file.read(reader))
    {
        std::cerr << "Elf file couldn't be read!" << std::endl;
        return nullptr;
    }

#define SYMBOL_VALUE(ident, name) \
    Elf::MinimumSymbol ident##_symbol = file.find_symbol(name); \
    if (ident##_symbol.Size == 0) \
    { \
        std::cerr << "Executable contains no " name " symbol!" << std::endl; \
        return nullptr; \
    } \
    size_t ident##_offset = file.address_to_offset(ident##_symbol.Value); \
    if (ident##_offset == 0) \
    { \
        std::cerr << name " offset invalid!" << std::endl; \
        return nullptr; \
    }

    SYMBOL_VALUE(ldata, "LData")
    SYMBOL_VALUE(hdata, "HData")
    SYMBOL_VALUE(level_name, "LevelName")
    SYMBOL_VALUE(cdata,"CData");

    std::vector<CharacterData> character_data;

    std::vector<size_t> level_name_offsets;
    reader.seek(ReaderBase::Start, level_name_offset);
    for (int i = 0; i < level_name_symbol.Size / 4; i++)
    {
        uint32_t value = reader.u32();
        size_t offset = file.address_to_offset(value);
        if (offset == 0)
        {
            std::cerr << "Something screwed up while reading level name pointers" << std::endl;
            return nullptr;
        }

        level_name_offsets.push_back(offset);
    }
    bool is_pal = level_name_offsets.size() == 264;

    std::vector<LevelData> level_data;
    if (file.find_symbol("E3TIMEOUT").Value != 0)
    {
        // E3
        size_t character_size = RawCharacterDataE3().size();
        int character_count = cdata_symbol.Size / character_size;
        for (int i = 0; i < character_count; i++)
        {
            RawCharacterDataE3 raw;

            reader.seek(ReaderBase::Start, cdata_offset + i * character_size);
            if (!raw.read(reader))
            {
                std::cerr << "Failed to read character data item " << i << std::endl;
                return nullptr;
            }

            reader.seek(ReaderBase::Start, file.address_to_offset(raw.p_path()));
            std::string path = reader.zero_terminated_string();

            reader.seek(ReaderBase::Start, file.address_to_offset(raw.p_file()));
            std::string filepath = reader.zero_terminated_string();

            reader.seek(ReaderBase::Start, file.address_to_offset(raw.p_name()));
            std::string name = reader.zero_terminated_string();
            character_data.push_back(CharacterData(raw, path, filepath, name));
        }

        int level_count = ldata_symbol.Size / RawLevelDataE3().size();
        std::vector<RawLevelDataE3> raw_levels;
        raw_levels.resize(level_count);

        reader.seek(ReaderBase::Start, ldata_offset);
        for (int i = 0; i < level_count; i++)
        {
            if (!raw_levels[i].read(reader))
            {
                std::cerr << "Failed to read level data item " << i << std::endl;
                return nullptr;
            }
        }

        for (int i = 0; i < level_count; i++)
        {
            std::vector<std::uint8_t> modellist;

            RawLevelDataE3 raw = raw_levels[i];
            size_t filepath_offset = file.address_to_offset(raw.p_filepath());
            size_t modellist_offset = file.address_to_offset(raw.p_modellist());

            reader.seek(ReaderBase::Start, filepath_offset);
            std::string filepath = reader.zero_terminated_string();

            reader.seek(ReaderBase::Start, modellist_offset);
            for (;;) {
                uint8_t v = reader.u8();
                if (v == 0xff || reader.status() != ReaderStatus::Ok)
                    break;
                modellist.push_back(v);
            }

            std::vector<std::string> names;
            for (int j = 0; j < 2; j++)
            {
                reader.seek(ReaderBase::Start, level_name_offsets[(i * 2) + j]);
                names.push_back(reader.zero_terminated_string());
            }

            LevelData item(raw, filepath, names, modellist);
            level_data.push_back(item);
        }
    } else {
        size_t character_size = RawCharacterData().size();
        int character_count = cdata_symbol.Size / character_size;
        for (int i = 0; i < character_count; i++)
        {
            RawCharacterData raw;

            reader.seek(ReaderBase::Start, cdata_offset + i * character_size);
            if (!raw.read(reader))
            {
                std::cerr << "Failed to read character data item " << i << std::endl;
                return nullptr;
            }

            reader.seek(ReaderBase::Start, file.address_to_offset(raw.p_path()));
            std::string path = reader.zero_terminated_string();

            reader.seek(ReaderBase::Start, file.address_to_offset(raw.p_file()));
            std::string filename = reader.zero_terminated_string();

            reader.seek(ReaderBase::Start, file.address_to_offset(raw.p_name()));
            std::string name = reader.zero_terminated_string();

            character_data.push_back(CharacterData(raw, path, filename, name));
        }

        int level_count = ldata_symbol.Size / RawLevelData().size();

        std::vector<RawLevelData> raw_levels;
        raw_levels.resize(level_count);

        reader.seek(ReaderBase::Start, ldata_offset);
        for (int i = 0; i < level_count; i++)
        {
            if (!raw_levels[i].read(reader))
            {
                std::cerr << "Failed to read level data item " << i << std::endl;
                return nullptr;
            }
        }

        for (int i = 0; i < level_count; i++)
        {
            std::vector<std::uint8_t> modellist;

            RawLevelData raw = raw_levels[i];
            size_t filepath_offset = file.address_to_offset(raw.p_filepath());
            size_t modellist_offset = file.address_to_offset(raw.p_modellist());

            reader.seek(ReaderBase::Start, filepath_offset);
            std::string filepath = reader.zero_terminated_string();

            reader.seek(ReaderBase::Start, modellist_offset);
            for (;;) {
                uint8_t v = reader.u8();
                if (v == 0xff || reader.status() != ReaderStatus::Ok)
                    break;
                modellist.push_back(v);
            }

            std::vector<std::string> names;
            for (int j = 0; j < (is_pal ? 6 : 1); j++)
            {
                reader.seek(ReaderBase::Start, level_name_offsets[i * (is_pal ? 6 : 1) + j]);
                names.push_back(reader.zero_terminated_string());
            }

            LevelData item(raw, filepath, names, modellist);
            level_data.push_back(item);
        }
    }

    reader.seek(ReaderBase::Start, hdata_offset);
    std::vector<uint8_t> hdata = reader.array<std::uint8_t>(hdata_symbol.Size);

    return new ElfExecutable(level_data, hdata, character_data);
}

}
