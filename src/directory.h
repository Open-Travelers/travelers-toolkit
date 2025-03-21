#pragma once

#include <memory>
#include <string>
#include <filesystem>
#include "twoc/binary_reader.h"

struct DirectoryEntry {
    bool IsDirectory;
    std::string Name;
};

class Directory {
protected:
    std::filesystem::path m_relative_path { "" };

    virtual bool cd_element(std::string const& dir_name) = 0;
    virtual bool cd_up() = 0;
public:
    Directory() {}
    virtual ~Directory() {}

    virtual std::unique_ptr<Twoc::BinaryReader> file_open(std::filesystem::path const& relative_path, Twoc::ReaderEndianness endianness) = 0;
    virtual std::filesystem::path file_find(std::filesystem::path const& relative_path) = 0;
    virtual bool file_exists(std::filesystem::path const& relative_path) = 0;

    virtual std::unique_ptr<Directory> fork(std::filesystem::path const& relative_path) = 0;

    bool cd(std::filesystem::path const& relative_path);
    virtual size_t file_count() = 0;
    virtual size_t directory_count() = 0;
    virtual size_t entry_count() = 0;

    virtual std::filesystem::path full_path() = 0;
    std::filesystem::path const& relative_path() { return m_relative_path; }
};
