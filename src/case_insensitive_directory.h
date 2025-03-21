#pragma once

#include <map>
#include <vector>
#include "directory.h"

class CaseInsensitiveDirectory : public Directory {
protected:
    bool m_case_override { false };
    std::filesystem::path m_root_path { "" };

    size_t m_directories { 0 };
    size_t m_files { 0 };

    std::map<std::string, size_t> m_entry_map;
    std::vector<DirectoryEntry> m_entries;

    bool repopulate();
    bool cd_element(std::string const& dir_name) override;
    bool cd_up() override;

public:
    CaseInsensitiveDirectory(std::filesystem::path const& path);
    ~CaseInsensitiveDirectory() override;

    std::filesystem::path file_find(std::filesystem::path const& path) override;
    std::unique_ptr<Twoc::BinaryReader> file_open(std::filesystem::path const& path, Twoc::ReaderEndianness endianness) override;
    bool file_exists(std::filesystem::path const& path) override;

    std::unique_ptr<Directory> fork(std::filesystem::path const& path) override;

    size_t file_count() override;
    size_t directory_count() override;
    size_t entry_count() override;

    std::filesystem::path full_path() override;
};
