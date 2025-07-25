
#include "texture.h"
#include "vertex.h"
#include "memory_binary_reader.h"

namespace Twoc::Nu 
{

static uint32_t dxt_blend(uint32_t v1, uint32_t v2)
{
    return (v1 * 3 + v2 * 5) >> 3;
}

static uint8_t convert_3to8(uint8_t v)
{
    return (uint8_t)((v << 5) | (v << 2) | (v >> 1));
}

static uint8_t convert_4to8(uint8_t v)
{
    return (uint8_t)((v << 4) | v);
}

static uint8_t convert_5to8(uint8_t v)
{
    return (uint8_t)((v << 3) | (v >> 2));
}

static uint8_t convert_6to8(uint8_t v)
{
    return (uint8_t)((v << 2) | (v >> 4));
}

static bool decode_rgb5a3_block(Twoc::BinaryReader &reader, Twoc::Nu::Color &color)
{
    uint16_t c = reader.u16();
    uint8_t r, g, b, a;

    if ((c & 0x8000) != 0)
    {
        r = convert_5to8((c >> 10) & 0x1f);
        g = convert_5to8((c >> 5) & 0x1f);
        b = convert_5to8(c & 0x1f);
        a = 0xFF;
    } else {
        a = convert_3to8((c >> 12) & 0x7);
        b = convert_4to8((c >> 8) & 0xf);
        g = convert_4to8((c >> 4) & 0xf);
        r = convert_4to8(c & 0xf);
    }

    color.Alpha = a;
    color.Red = r;
    color.Green = g;
    color.Blue = b;
    return reader.status() != Twoc::ReaderStatus::Error;
}

static uint32_t pixel_size(uint32_t type)
{
    switch (type)
    {
    case 0:
    case 1:
        return 0x10;
    case 2:
        return 0x18;
    case 3:
        return 0x20;
    case 4:
        return 4;
    case 5:
        return 8;
    default:
        return 0;
    }
}

static uint32_t texture_size_mm(uint32_t type, uint32_t width, uint32_t height, uint32_t mipmaps)
{
    uint32_t size = width * height * pixel_size(type) >> 3;
    if (mipmaps > 0)
    {
        uint32_t i = 0;
        do {
            i++;
            uint32_t additional = pixel_size(type);
            size = size + ((width >> (i & 0x1f)) * (height >> (i & 0x1f)) * additional >> 3);
        } while (i < mipmaps);
    }
    return size;
}

static void decode_raw_rgb8(Twoc::BinaryReader &reader, std::vector<uint8_t> &rgba_data, uint32_t texture_size)
{
    int j = 0;
    for (uint32_t i = 0; i < texture_size; i += 3)
    {
        uint8_t r = reader.u8();
        uint8_t g = reader.u8();
        uint8_t b = reader.u8();
        rgba_data.push_back(b);
        rgba_data.push_back(g);
        rgba_data.push_back(r);
        rgba_data.push_back(255);
        j++;
    }
}

static void decode_raw_argb8(Twoc::BinaryReader &reader, std::vector<uint8_t> &rgba_data, uint32_t texture_size)
{
    for (uint32_t i = 0; i < texture_size; i += 4)
    {
        uint8_t a = reader.u8();
        uint8_t r = reader.u8();
        uint8_t g = reader.u8();
        uint8_t b = reader.u8();
        rgba_data.push_back(b);
        rgba_data.push_back(g);
        rgba_data.push_back(r);
        rgba_data.push_back(a);
    }
}

static void decode_palletized_argb8(Twoc::BinaryReader &reader, std::vector<uint8_t> const& palette, std::vector<uint8_t> &rgba_data, uint32_t texture_size, uint32_t type)
{   
    for (int i = 0; i < texture_size; i++)
    {
        uint8_t idx;
        if (type == 4)
        {
            reader.seek(ReaderBase::Start, i >> 1);
            if ((i & 1) == 0)
            {
                idx = reader.u8() >> 4;
            } else {
                idx = reader.u8() & 0xf;
            }
        } else {
            reader.seek(ReaderBase::Start, i);
            idx = reader.u8();
        }
        
        uint8_t a = palette[idx * 4];
        uint8_t r = palette[idx * 4 + 1];
        uint8_t g = palette[idx * 4 + 2];
        uint8_t b = palette[idx * 4 + 3];
        rgba_data.push_back(b);
        rgba_data.push_back(g);
        rgba_data.push_back(r);
        rgba_data.push_back(a);
    }
}

static void decode_dxt_block(Twoc::BinaryReader &reader, std::vector<uint8_t> &rgba_data, uint32_t width, uint32_t block_x, uint32_t block_y)
{
    uint16_t c1 = reader.u16();
    uint16_t c2 = reader.u16();
    std::vector<uint8_t> lines = reader.array<uint8_t>(4);

    uint8_t blue1 = convert_5to8(c1 & 0x1F);
    uint8_t blue2 = convert_5to8(c2 & 0x1F);
    uint8_t green1 = convert_6to8((c1 >> 5) & 0x3F);
    uint8_t green2 = convert_6to8((c2 >> 5) & 0x3F);
    uint8_t red1 = convert_5to8((c1 >> 11) & 0x1F);
    uint8_t red2 = convert_5to8((c2 >> 11) & 0x1F);

    Twoc::Nu::Color colors[4];
    colors[0].Alpha = 255;
    colors[0].Red = red1;
    colors[0].Green = green1;
    colors[0].Blue = blue1;

    colors[1].Alpha = 255;
    colors[1].Red = red2;
    colors[1].Green = green2;
    colors[1].Blue = blue2;

    if (c1 > c2)
    {
        colors[2].Red = dxt_blend(red2, red1);
        colors[2].Green = dxt_blend(green2, green1);
        colors[2].Red = dxt_blend(blue2, blue1);
        colors[2].Alpha = 255;

        colors[3].Red = dxt_blend(red1, red2);
        colors[3].Green = dxt_blend(green1, green2);
        colors[3].Red = dxt_blend(blue1, blue2);
        colors[3].Alpha = 255;
    } else {
        colors[2].Red = (red1 + red2) / 2;
        colors[2].Green = (green1 + green2) / 2;
        colors[2].Blue = (blue1 + blue2) / 2;
        colors[2].Alpha = 255;

        colors[3].Red = colors[2].Red;
        colors[3].Green = colors[2].Green;
        colors[3].Blue = colors[2].Blue;
        colors[3].Alpha = 0;
    }

    for (uint32_t y = 0; y < 4; y++)
    {
        uint8_t line = lines[y];
        for (uint32_t x = 0; x < 4; x++)
        {
            auto color = colors[(line >> 6) & 3];
            size_t offset = ((x + block_x) + (y + block_y) * width) * 4;
            rgba_data[offset + 0] = color.Red;
            rgba_data[offset + 1] = color.Green;
            rgba_data[offset + 2] = color.Blue;
            rgba_data[offset + 3] = color.Alpha;
            line <<= 2;
        }
    }
}

static void decode_dxt(Twoc::BinaryReader &reader, std::vector<uint8_t> &rgba_data, uint32_t texture_size, uint32_t width, uint32_t height)
{
    for (uint32_t y = 0; y < height; y += 8)
    {
        for (uint32_t x = 0; x < width; x += 8)
        {
            decode_dxt_block(reader, rgba_data, width, x, y);
            decode_dxt_block(reader, rgba_data, width, x + 4, y);
            decode_dxt_block(reader, rgba_data, width, x, y + 4);
            decode_dxt_block(reader, rgba_data, width, x + 4, y + 4);
        }
    }
}

static void decode_rgb5a3(Twoc::BinaryReader &reader, std::vector<uint8_t> &rgba_data, uint32_t texture_size, uint32_t width, uint32_t height)
{
    for (uint32_t y = 0; y < height; y += 4)
    {
        for (uint32_t x = 0; x < width; x += 4)
        {
            for (uint32_t block_y = 0; block_y < 4; block_y++)
            {
                for (uint32_t block_x = 0; block_x < 4; block_x++)
                {
                    Twoc::Nu::Color color;
                    decode_rgb5a3_block(reader, color);

                    rgba_data[((x + block_x) + (y + block_y) * width) * 4] = color.Red;
                    rgba_data[((x + block_x) + (y + block_y) * width) * 4 + 1] = color.Green;
                    rgba_data[((x + block_x) + (y + block_y) * width) * 4 + 2] = color.Blue;
                    rgba_data[((x + block_x) + (y + block_y) * width) * 4 + 3] = color.Alpha;
                }
            }
        }
    }
}

Bitmap::Bitmap(uint32_t width, uint32_t height, std::vector<uint8_t> data) : m_width(width), m_height(height), m_data(data) { }

static size_t palette_size_mm(uint32_t type)
{
    if (type == 4)
        return 0x40;
    else if (type == 5)
        return 0x400;
    else
        return 0;
}

bool Texture::read(Twoc::BinaryReader &reader, size_t fullsize)
{
    auto type = reader.u32();
    auto width = reader.u32();
    auto height = reader.u32();
    auto mipmap_count = reader.u32();
    if (mipmap_count == 1)
        mipmap_count = 0;

    auto palette_size = palette_size_mm(type);
    std::cout << "Reading texture of type " << std::to_string(type) << " of width " << std::to_string(width) << " and height " << std::to_string(height) << ". Mipmaps: " << std::to_string(mipmap_count) << ", palette: " << std::to_string(palette_size) << std::endl;
    
    auto texture_size = texture_size_mm(type, width, height, mipmap_count);
    auto texture_data = reader.array<uint8_t>(texture_size);
    
    MemoryBinaryReader texture_reader(reader.file_endianness());
    if (!texture_reader.open(texture_data.data(), texture_size))
        return false;

    std::vector<uint8_t> rgba_data;
    rgba_data.reserve(width * height * 4);

    switch (type)
    {
    case 2: decode_raw_rgb8(texture_reader, rgba_data, texture_size); break;
    case 3: decode_raw_argb8(texture_reader, rgba_data, texture_size); break;
    case 4: case 5:
    {
        auto palette_data = reader.array<uint8_t>(palette_size); 
        
        if (type == 4)
            texture_size *= 2;
    
        decode_palletized_argb8(texture_reader, palette_data, rgba_data, texture_size, type);
    } break;
    case 0x80: decode_dxt(texture_reader, rgba_data, texture_size, width, height); break;
    case 0x81: decode_rgb5a3(texture_reader, rgba_data, texture_size, width, height); break;
    default:
        std::cerr << "Unsupported texture type " << std::to_string(type) << "!" << std::endl;
        return false;
    }

    m_mipmaps.push_back(Bitmap(width, height, rgba_data));
    m_type = type;
    m_max_width = width;
    m_max_height = height;
    return true;
}

#if 0
bool Texture::read(Twoc::BinaryReader &reader, size_t fullsize)
{ 
    (void) fullsize;
    std::vector<uint8_t> palette_data;
    std::vector<uint8_t> texture_data;
    uint32_t type = reader.u32();
    uint32_t width = reader.u32();
    uint32_t height = reader.u32();
    uint32_t mipmap_count = reader.u32();
    if (mipmap_count == 1) 
        mipmap_count = 0;

    uint32_t texture_size = texture_size_mm(type, width, height, mipmap_count);
    if ((type & 0x80) != 0)
    {
        texture_size = mipmap_count;
        mipmap_count = 0;
    }

    size_t palette_size = palette_size_mm(type);

    texture_data = reader.array<uint8_t>(texture_size);
    if (palette_size != 0)
        palette_data = reader.array<uint8_t>(palette_size);

    std::vector<uint8_t> rgba_data(width * height * 4);
    
    MemoryBinaryReader texture_reader(reader.file_endianness());
    if (!texture_reader.open(texture_data.data(), texture_size))
        return false;

    switch (type)
    {
    case 2: decode_raw_rgb8(texture_reader, rgba_data, texture_size); break;
    case 3: decode_raw_argb8(texture_reader, rgba_data, texture_size); break;

    case 4: case 5: {
        if (type == 4)
            texture_size *= 2;
        decode_palletized_argb8(texture_reader, palette_data, rgba_data, texture_size, type);
    } break;

    case 0x80: decode_dxt(texture_reader, rgba_data, texture_size, width, height); break;
    case 0x81: decode_rgb5a3(texture_reader, rgba_data, texture_size, width, height); break;
    default:
        std::cerr << "Unsupported texture type " << std::to_string(type) << "!" << std::endl;
        return false;
    }

    m_mipmaps.push_back(Bitmap(width, height, rgba_data));
    m_type = type;
    m_max_width = width;
    m_max_height = height;
    std::cout << "Texture of type " << std::to_string(type) << " of width " << std::to_string(width) << " and height " << std::to_string(height) << ". Mipmaps: " << std::to_string(mipmap_count) << ", palette: " << std::to_string(palette_size) << ", tex size: " << std::to_string(rgba_data.size()) << std::endl;
    return true; 
}
#endif
bool Texture::validate() { return m_mipmaps.size() != 0; }


}
