#pragma once

#include <string>
#include <set>
#include <filesystem>
#include "twoc/executable.h"
#include "file_binary_reader.h"

enum class ProjectBaseConsole {
    NONE = -1,
    PS2 = 0,
    GAMECUBE,
    XBOX
};

class ApplicationProject {
protected:
    ProjectBaseConsole m_console { ProjectBaseConsole::NONE };
    Twoc::Executable *m_executable { nullptr };
    Twoc::ReaderEndianness m_endianness;
    std::set<int> m_level_existance;
    std::filesystem::path m_root_path;

public:
    ApplicationProject() = default;
    ~ApplicationProject();

    bool load(std::string const& directory);
    void unload();

    FileBinaryReader find_file(std::filesystem::path const& path);

    bool does_level_exist(int index);
    Twoc::Executable const* executable() const { return m_executable; }
    Twoc::ReaderEndianness endianness() const { return m_endianness; }
    std::filesystem::path const& root_path() const { return m_root_path; }
};
