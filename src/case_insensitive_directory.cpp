#include <iostream>
#include <filesystem>
#include <algorithm>
#include "file_binary_reader.h"
#include "case_insensitive_directory.h"

namespace fs = std::filesystem;
CaseInsensitiveDirectory::CaseInsensitiveDirectory(std::filesystem::path const& path) : m_root_path(path)
{
    std::cout << "Root path: " << m_root_path << std::endl;
    repopulate();
}

CaseInsensitiveDirectory::~CaseInsensitiveDirectory()
{

}

fs::path CaseInsensitiveDirectory::file_find(fs::path const& relative_path)
{
    // is path just a filename?
    // yes:
    //    return real file path if it exists
    // no:
    //    fork directory with path stem
    //    recurse in forked directory with filename
    if (!relative_path.has_filename())
        return {};

    std::string temp = relative_path.string();
    std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

    fs::path new_relative_path(temp);

    auto distance = std::distance(new_relative_path.begin(), new_relative_path.end());
    if (distance == 1)
    {
        if (m_entry_map.find(new_relative_path.string()) == m_entry_map.end())
            return {};

        auto entry = m_entries[m_entry_map[new_relative_path.string()]];
        return full_path() / entry.Name;
    } else if (distance > 1)
    {
        auto relative_directory = new_relative_path;
        relative_directory.remove_filename();

        std::unique_ptr<Directory> forked = fork(relative_directory);
        return forked->file_find(new_relative_path.filename());
    } else {
        return {};
    }
    return {};
}

std::unique_ptr<Twoc::BinaryReader> CaseInsensitiveDirectory::file_open(std::filesystem::path const& relative_path, Twoc::ReaderEndianness endianness)
{
    fs::path real_path = file_find(relative_path);
    if (real_path.empty())
        return {};

    std::unique_ptr<FileBinaryReader> reader = std::make_unique<FileBinaryReader>(endianness);
    if (!reader->open(real_path))
        return {};

    return reader;
}

bool CaseInsensitiveDirectory::file_exists(std::filesystem::path const& relative_path)
{
    // is path just a filename?
    // yes:
    //     return if filename exists
    // not:
    //     fork directory with path stem    repopulate();    repopulate();
    //     recurse in forked directory with filename
    if (!relative_path.has_filename())
    {
        std::unique_ptr<Directory> forked = fork(relative_path);
        if (forked)
            return true;
        return false;
    }

    auto distance = std::distance(relative_path.begin(), relative_path.end());
    if (distance == 1)
    {
        return m_entry_map.find(relative_path.string()) != m_entry_map.end();
    } else if (distance > 1)
    {
        auto relative_directory = relative_path;
        relative_directory.remove_filename();

        std::unique_ptr<Directory> forked = fork(relative_directory);
        return forked->file_exists(relative_path.filename());
    } else {
        return false;
    }
    return false;
}

std::unique_ptr<Directory> CaseInsensitiveDirectory::fork(std::filesystem::path const& relative_path)
{
    // is path absolute?
    //     exit
    // does path have filename?
    //     exit
    // create new directory structure
    // loop through relative path elements
    // change directory to each in sequence

    if (!relative_path.is_relative())
        return {};

    if (relative_path.has_filename())
        return {};

    std::unique_ptr<Directory> directory = std::make_unique<CaseInsensitiveDirectory>(m_root_path);
    for (auto const& element : relative_path)
    {
        if (element.empty())
            break;

        if (!directory->cd(element))
            return {};
    }
    return directory;
}

size_t CaseInsensitiveDirectory::file_count()
{
    return m_files;
}

size_t CaseInsensitiveDirectory::directory_count()
{
    return m_directories;
}

size_t CaseInsensitiveDirectory::entry_count()
{
    return m_entries.size();
}

bool CaseInsensitiveDirectory::repopulate()
{
    std::vector<DirectoryEntry> entries;
    std::map<std::string, size_t> entry_map;
    size_t directories = 0, files = 0;

    std::cout << "Repopulating from " << full_path() << std::endl;
    for (auto const& real_entry : fs::directory_iterator(full_path()))
    {
        std::string filename = real_entry.path().filename();

        DirectoryEntry entry;
        entry.IsDirectory = real_entry.is_directory();
        entry.Name = filename;

        std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
        if (entry_map.find(filename) != entry_map.end())
            return false;

        // std::cout << "Mapping " << entry.Name << " -> " << filename << std::endl;
        entry_map[filename] = entries.size();
        entries.push_back(entry);

        if (entry.IsDirectory)
            directories++;
        else
            files++;
    }

    m_directories = directories;
    m_files = files;
    m_entries = entries;
    m_entry_map = entry_map;
    return true;
}

bool CaseInsensitiveDirectory::cd_up()
{
    return false;
}

bool CaseInsensitiveDirectory::cd_element(std::string const& dir_name)
{
    // get case-sensitive directory name from name map
    // does it exist?
    // yes:
    //     add real directory name to relative path
    //     repopulate
    // no:
    //     exit

    if (m_entry_map.find(dir_name) == m_entry_map.end())
        return false;

    DirectoryEntry entry = m_entries[m_entry_map[dir_name]];

    std::filesystem::path relative_path(m_relative_path);
    m_relative_path /= entry.Name;

    if (!repopulate())
    {
        m_relative_path = relative_path;
        return false;
    }
    return true;
}

std::filesystem::path CaseInsensitiveDirectory::full_path()
{
    fs::path path(m_root_path);
    path /= m_relative_path;
    return path;
}
