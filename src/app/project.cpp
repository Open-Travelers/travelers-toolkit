#include <stdexcept>
#include <filesystem>
#include <portable-file-dialogs.h>
#include "../file_binary_reader.h"
#include "project.h"
#include "case_insensitive_directory.h"

namespace App {

Project::~Project()
{

}

bool Project::load(const std::string &directory)
{
    unload();

    std::shared_ptr<Directory> root_dir = std::make_shared<CaseInsensitiveDirectory>(directory);
    if (root_dir->file_exists("default.xbe"))
    {
        (void) pfd::message("Fatal", "Xbox version not yet supported!", pfd::choice::ok, pfd::icon::error);
        return false;
    } else if (root_dir->file_exists("crashwoc.elf"))
    {
#if 1
        auto reader = root_dir->file_open("crashwoc.elf", Twoc::ReaderEndianness::Big);
        if (!reader)
        {
            (void) pfd::message("Fatal", "Gamecube executable exists but could not be opened!", pfd::choice::ok, pfd::icon::error);
            return false;
        }
        
        auto *executable_file = Twoc::ElfExecutable::from_reader(*reader);
        if (!executable_file)
        {
            (void) pfd::message("Fatal", "Gamecube executable couldn't be parsed!", pfd::choice::ok, pfd::icon::error);
            return false;
        }

        m_executable = executable_file;
        m_endianness = Twoc::ReaderEndianness::Big;
#else
        return false;
#endif
    } else if (root_dir->file_exists("system.cnf"))
    {
        std::filesystem::path cnf_path = root_dir->file_find("system.cnf");

        std::ifstream cnf(cnf_path);
        if (!cnf.is_open())
        {
            (void) pfd::message("Fatal", "SYSTEM.CNF file exists, but couldn't be opened!", pfd::choice::ok, pfd::icon::error);
            return false;
        }

        // Different PS2 revisions will have differently named executable files.
        // Parsing system.cnf file will get that executable file's name.
        std::regex boot_regex("^\\w+\\s*=\\s*\\w+:\\\\(\\w\\w\\w\\w_\\d\\d\\d\\.\\d\\d);\\d");
        std::string line;
        std::string executable_filename;
        while (std::getline(cnf, line))
        {
            std::smatch match;
            if (std::regex_search(line, match, boot_regex))
            {
                executable_filename = match[1];
                break;
            }
        }

        auto reader = root_dir->file_open(executable_filename, Twoc::ReaderEndianness::Little);
        if (!reader)
        {
            (void) pfd::message("Fatal", "PS2 executable doesn't exist or couldn't be found!", pfd::choice::ok, pfd::icon::error);
            return false;
        }

        auto *exe = Twoc::ElfExecutable::from_reader(*reader);
        if (!exe)
        {
            (void) pfd::message("Fatal", "PS2 executable couldn't be parsed!", pfd::choice::ok, pfd::icon::error);
            return false;
        }

        m_executable = exe;
        m_endianness = Twoc::ReaderEndianness::Little;
    } else {
        return false;
    }


    std::filesystem::path level_path = "levels/";
    int i = 0;
    for (auto const& level : m_executable->level_data())
    {
        auto corrected_path = std::regex_replace(level.Filepath, std::regex("\\\\"), "/");
        auto path_object = (level_path / std::filesystem::path(corrected_path)).make_preferred();

        auto directory = path_object.remove_filename();
        if (root_dir->file_exists(directory))
            m_level_existance.insert(i);
        i++;
    }

    m_root_directory = root_dir;
    return true;
}

std::unique_ptr<Twoc::BinaryReader> Project::find_file(std::filesystem::path const& path)
{
    std::filesystem::path fixed_path(std::regex_replace(path.string(), std::regex("\\\\"), "/"));
    fixed_path = fixed_path.make_preferred();
    return m_root_directory->file_open(fixed_path, m_endianness);
}

void Project::unload()
{
    m_level_existance.clear();

    delete m_executable;
    m_executable = nullptr;
    m_endianness = Twoc::ReaderEndianness::Little;
}

bool Project::does_level_exist(int index)
{
    return m_level_existance.contains(index);
}

}
