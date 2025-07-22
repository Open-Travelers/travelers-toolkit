#ifndef TEXTURE_H
#define TEXTURE_H

#include "../reader_item.h"

namespace Twoc::Nu 
{

class Bitmap
{
protected:
    uint32_t m_width { 0 };
    uint32_t m_height { 0 };
    uint32_t m_type { 0 };
    std::vector<uint8_t> m_data;

public:
    Bitmap(uint32_t width, uint32_t height, std::vector<uint8_t> data);
    std::vector<uint8_t> const& data() const { return m_data; }
    uint8_t const* raw_data() const { return m_data.data(); }
};

class Texture : public IReaderItem
{
protected:
    uint32_t m_type;
    uint32_t m_max_width;
    uint32_t m_max_height;
    std::vector<Bitmap> m_mipmaps;

public:
    size_t size() override { return 0; }
    bool read(Twoc::BinaryReader &reader, size_t fullsize = 0) override;
    bool validate() override;

    Bitmap const& bitmap(size_t i) const { return m_mipmaps[i]; }
    size_t bitmap_count() const { return m_mipmaps.size(); }

    uint32_t max_width() const { return m_max_width; }
    uint32_t max_height() const { return m_max_height; }
    uint32_t type() const { return m_type; }
};

}
#endif
