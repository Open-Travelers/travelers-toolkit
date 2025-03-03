#ifndef BINARY_READER_OLD_H
#define BINARY_READER_OLD_H

#include <cstdint>
#include <string>
#include <vector>

namespace Twoc
{

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

class IBinaryReader {
public:
    IBinaryReader(ReaderEndianness endianness);
    virtual ~IBinaryReader() = default;

    virtual bool open(std::string const& path) = 0;
    virtual void close() = 0;

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

    virtual std::uint8_t u8() = 0;
    virtual std::uint16_t u16() = 0;
    virtual std::uint32_t u32() = 0;
    virtual float f32() = 0;

    std::int8_t s8()
    {
        return u8();
    }
    std::int16_t s16()
    {
        return u16();
    }
    std::int32_t s32()
    {
        return u32();
    }
    virtual void* ptr(ptrdiff_t initial_base, void* target_base) = 0;

    virtual std::vector<std::uint8_t> array(size_t size) = 0;
    virtual std::string zero_terminated_string() = 0;

    virtual std::size_t tell() = 0;
    virtual std::size_t seek(ReaderBase base, std::size_t offset) = 0;

    ReaderStatus status() const { return m_status; }
    ReaderEndianness endianness() const { return m_endianness; }

protected:
    ReaderEndianness m_endianness { ReaderEndianness::Little };
    ReaderStatus m_status { ReaderStatus::Ok };
};

}
#endif // BINARY_READER_OLD_H
