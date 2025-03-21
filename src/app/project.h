#pragma once

#include <string>
#include <set>
#include <filesystem>
#include "../twoc/executable.h"
#include "../file_binary_reader.h"
#include "../directory.h"

namespace App {

enum class ProjectBaseConsole {
    NONE = -1,
    PS2 = 0,
    GAMECUBE,
    XBOX
};

class Project {
protected:
    ProjectBaseConsole m_console { ProjectBaseConsole::NONE };
    Twoc::Executable *m_executable { nullptr };
    Twoc::ReaderEndianness m_endianness;
    std::set<int> m_level_existance;
    std::shared_ptr<Directory> m_root_directory;

public:
    Project() = default;
    ~Project();

    bool load(std::string const& directory);
    void unload();

    std::unique_ptr<Twoc::BinaryReader> find_file(std::filesystem::path const& path);

    bool does_level_exist(int index);
    Twoc::Executable const* executable() const { return m_executable; }
    Twoc::ReaderEndianness endianness() const { return m_endianness; }
    std::filesystem::path root_path() const { return m_root_directory->full_path(); }
};

}
