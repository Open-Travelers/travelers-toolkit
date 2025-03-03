#include <stdexcept>
#include <filesystem>
#include <portable-file-dialogs.h>
#include "file_binary_reader.h"
#include "application_project.h"

ApplicationProject::~ApplicationProject()
{

}

bool ApplicationProject::load(const std::string &directory)
{
    unload();

    std::filesystem::path root_path(directory);
    if (std::filesystem::exists(root_path / "default.xbe"))
    {
        (void) pfd::message("Fatal", "Xbox version not yet supported!", pfd::choice::ok, pfd::icon::error);
        return false;
    } else if (std::filesystem::exists(root_path / "crashwoc.elf"))
    {
        FileBinaryReader executable_file(Twoc::ReaderEndianness::Big);
        if (!executable_file.open(root_path / "crashwoc.elf"))
        {
            (void) pfd::message("Fatal", "Gamecube executable exists but could not be opened!", pfd::choice::ok, pfd::icon::error);
            return false;
        }

        Twoc::ElfExecutable *exe = Twoc::ElfExecutable::from_reader(executable_file);
        if (!exe)
        {
            (void) pfd::message("Fatal", "Gamecube executable couldn't be parsed!", pfd::choice::ok, pfd::icon::error);
            return false;
        }

        m_executable = exe;
        m_endianness = Twoc::ReaderEndianness::Big;
    } else if (std::filesystem::exists(root_path / "system.cnf"))
    {
        std::ifstream cnf(root_path / "system.cnf");
        if (!cnf.is_open())
        {
            (void) pfd::message("Fatal", "SYSTEM.CNF file exists, but couldn't be opened!", pfd::choice::ok, pfd::icon::error);
            return false;
        }

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

        FileBinaryReader executable(Twoc::ReaderEndianness::Little);

        bool done = false;
        if (executable.open(root_path / executable_filename))
            done = true;

        std::transform(executable_filename.begin(), executable_filename.end(), executable_filename.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        if(!done && executable.open(root_path / executable_filename))
            done = true;

        if (!done)
        {
            (void) pfd::message("Fatal", "PS2 executable exists but could not be opened!", pfd::choice::ok, pfd::icon::error);
            return false;
        }

        Twoc::ElfExecutable *exe = Twoc::ElfExecutable::from_reader(executable);
        if (!exe)
        {
            (void) pfd::message("Fatal", "PS2 executable couldn't be parsed!", pfd::choice::ok, pfd::icon::error);
            return false;
        }

        m_executable = exe;
        m_endianness = Twoc::ReaderEndianness::Little;
    }

    int i = 0;
    for (auto const& level : m_executable->level_data())
    {
        auto level_path = root_path / "levels";
        auto corrected_path = std::regex_replace(level.Filepath, std::regex("\\\\"), "/");
        auto path_object = std::filesystem::path(corrected_path).make_preferred();
        auto directory = level_path / path_object.remove_filename();
        if (std::filesystem::exists(directory))
            m_level_existance.insert(i);
        i++;
    }

    m_root_path = directory;
    return true;
}

FileBinaryReader find_file(std::filesystem::path const& path)
{
    /*
    std::filesystem::path fixed_path(std::regex_replace(path.string(), std::regex("\\\\"), "/"));
    fixed_path = fixed_path.make_preferred();

    std::string file_name = fixed_path.filename().string();
    fixed_path = fixed_path.remove_filename();

    std::filesystem::path full_path = m_root_path / fixed_path;
    FileBinaryReader reader;
    std::transform(file_name.begin(), file_name.end(), file_name.begin(), ::tolower);
    if (!reader.open((full_path / file_name)))
    {
        reader.open(full_path);
    }
*/

}

void ApplicationProject::unload()
{
    m_level_existance.clear();

    delete m_executable;
    m_executable = nullptr;
    m_endianness = Twoc::ReaderEndianness::Little;
}

bool ApplicationProject::does_level_exist(int index)
{
    return m_level_existance.contains(index);
}
