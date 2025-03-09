#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace Twoc {

enum class ReaderEndianness {
    Little,
    Big
};

enum class ReaderStatus {
    Ok,
    Eof,
    Error
};

enum class ReaderBase {
    Start,
    Current,
    End
};

class BinaryReader {
protected:
    ReaderEndianness m_file_endianness { ReaderEndianness::Little };
    ReaderEndianness m_system_endianness { ReaderEndianness::Little };
    ReaderStatus m_status { ReaderStatus::Ok };

    virtual bool read_buffer(uint8_t *buffer, size_t size) = 0;
public:
    BinaryReader(ReaderEndianness endianness);
    virtual ~BinaryReader() {}

    virtual bool open(std::string const& path) = 0;
    virtual void close() = 0;
    virtual std::size_t tell() = 0;
    virtual std::size_t seek(ReaderBase base, std::size_t offset) = 0;

    std::string zero_terminated_string();
    std::uint8_t u8();
    std::uint16_t u16();
    std::uint32_t u32();
    float f32();
    void* ptr(ptrdiff_t initial_base, void* target_base);

    std::int8_t s8() { return u8(); }
    std::int16_t s16() { return u16(); }
    std::int32_t s32() { return u32(); }

    template<typename T>
    std::vector<T> array(size_t count)
    {
        std::vector<T> result;
        result.resize(count);
        for (size_t i = 0; i < count; i++)
            result[i] = read<T>();
        return result;
    }

    template<typename T>
    T read() {
        if constexpr(std::is_same<T, uint8_t>::value) {
            return u8();
        } else if constexpr(std::is_same<T, uint16_t>::value) {
            return u16();
        } else if constexpr(std::is_same<T, uint32_t>::value) {
            return u32();
        } else if constexpr(std::is_same<T, int8_t>::value || std::is_same<T, char>::value) {
            return s8();
        } else if constexpr(std::is_same<T, int16_t>::value) {
            return s16();
        } else if constexpr(std::is_same<T, int32_t>::value) {
            return s32();
        } else if constexpr(std::is_same<T, float>::value) {
            return f32();
        }
        return 0;
    }

    ReaderStatus status() const { return m_status; }
    ReaderEndianness file_endianness() const { return m_file_endianness; }
};
}
