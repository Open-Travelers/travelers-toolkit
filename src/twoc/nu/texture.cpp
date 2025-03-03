
#include "texture.h"

namespace Twoc::Nu 
{

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
Bitmap::Bitmap(uint32_t width, uint32_t height, std::vector<uint8_t> data) : m_width(width), m_height(height), m_data(data) { }

bool Texture::read(Twoc::BinaryReader &reader, size_t fullsize)
{ 
    (void) fullsize;
    uint32_t type = reader.u32();
    uint32_t width = reader.u32();
    uint32_t height = reader.u32();
    uint32_t mipmap_count = reader.u32();
        
    if (mipmap_count == 1) 
        mipmap_count = 0;

    uint32_t texture_size = texture_size_mm(type, width, height, mipmap_count);
    std::vector<uint8_t> texture_data = reader.array<uint8_t>(texture_size);

    size_t palette_size = 0;
    if (type == 4)
        palette_size = 0x40;
    else if (type == 5)
        palette_size = 0x400;

    std::vector<uint8_t> rgba_data;
    if (type == 4 || type == 5)
    {
        std::vector<uint8_t> palette_data = reader.array<uint8_t>(palette_size);
        if (type == 4)
            texture_size *= 2;

        for (uint32_t i = 0; i < texture_size; i++)
        {
            uint8_t idx;
            if (type == 4)
            {
                if ((i & 1) == 0)
                {
                    idx = texture_data[i >> 1] >> 4;
                } else {
                    idx = texture_data[i >> 1] & 0xf;
                }
            } else {
                idx = texture_data[i];
            }
            uint8_t a = palette_data[idx * 4];
            uint8_t r = palette_data[idx * 4 + 1];
            uint8_t g = palette_data[idx * 4 + 2];
            uint8_t b = palette_data[idx * 4 + 3];
            rgba_data.push_back(b);
            rgba_data.push_back(g);
            rgba_data.push_back(r);
            rgba_data.push_back(a);
        } 
    } else if (type == 2) {
        for (uint32_t i = 0; i < texture_size; i++)
        {
            uint8_t a = texture_data[i];
            uint8_t r = texture_data[i + 1];
            uint8_t g = texture_data[i + 2];
            uint8_t b = texture_data[i + 3];
            rgba_data.push_back(b);
            rgba_data.push_back(g);
            rgba_data.push_back(r);
            rgba_data.push_back(a);
        }
    } else {
        for (uint32_t i = 0; i < texture_size; i+=3)
        {
            uint8_t r = texture_data[i + 0];
            uint8_t g = texture_data[i + 1];
            uint8_t b = texture_data[i + 2];
            rgba_data.push_back(b);
            rgba_data.push_back(g);
            rgba_data.push_back(r);
            rgba_data.push_back(255);
        }
    }

    m_mipmaps.push_back(Bitmap(width, height, rgba_data));
    std::cout << "Texture of type " << std::to_string(type) << " of width " << std::to_string(width) << " and height " << std::to_string(height) << ". Mipmaps: " << std::to_string(mipmap_count) << ", palette: " << std::to_string(palette_size) << ", tex size: " << std::to_string(rgba_data.size()) << std::endl;
    return true; 
}

bool Texture::validate() { return m_mipmaps.size() != 0; }


}
