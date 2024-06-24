#ifndef BINARY_STREAM_H
#define BINARY_STREAM_H
#include <string>
#include <fstream>
#include <stdint.h>
#include <optional>
#include <vector>

class BinaryStream {
public:
    BinaryStream();
    ~BinaryStream();
    bool open(std::string const& path);
public:
    std::optional<uint8_t> read_byte();
    std::optional<uint16_t> read_half();
    std::optional<uint32_t> read_word();
    std::optional<float> read_float();
    std::optional<std::string> read_zero_terminated_string();
    std::optional<std::string> read_constant_length_string(size_t length);
    bool read(size_t size, uint8_t *output_buffer);
    
    void seek(int whence, long int offset);
    size_t tell();
    bool is_eof();
protected:
    std::string m_path;
    bool m_is_open { false };

    size_t m_position { 0 };
    size_t m_size { 0 };
    std::vector<uint8_t> m_data;
};
#endif
