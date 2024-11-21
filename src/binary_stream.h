#ifndef BINARY_STREAM_H
#define BINARY_STREAM_H
#include <string>
#include <cstdint>
#include <optional>
#include <vector>

class ReadableItem {
public:
    virtual void swap_endianness() = 0;
    virtual size_t size() = 0;
};

class BinaryStream {
public:
    explicit BinaryStream(bool swap_endianness = false);
    ~BinaryStream();
    bool open(std::string const& path);
public:
    std::optional<uint8_t> read_byte();
    std::optional<uint16_t> read_half();
    std::optional<uint32_t> read_word();
    std::optional<float> read_float();
    std::optional<std::string> read_zero_terminated_string();
    std::optional<std::string> read_constant_length_string(size_t length);
    
    template<std::derived_from<ReadableItem> T>
    std::optional<T> read_item()
    {
        T item;
        // not protected by squat
        if (read(sizeof(T), &item))
            return std::make_optional(item);
        
        if (m_should_swap_endianness)
            item.swap_endianness();
        return std::nullopt;
    }

    bool read(size_t size, uint8_t *output_buffer);
    
    void seek(int whence, long int offset);
    size_t tell();
    bool is_eof();
protected:
    bool m_should_swap_endianness { false };
    std::string m_path { "" };
    bool m_is_open { false };

    size_t m_position { 0 };
    size_t m_size { 0 };
    std::vector<uint8_t> m_data;
};
#endif
