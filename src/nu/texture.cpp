#include "texture.h"
#include <cstring>
#include <iostream>
#include <cstdlib>
namespace Nu {
    static const int pixel_size(int type)
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
    static const int texture_size_mm(int type, int width, int height, int mipmaps)
    {
        int size = width * height * pixel_size(type) >> 3;
        if (mipmaps > 0)
        {
            int i = 0;
            do {
                i++;
                int additional = pixel_size(type);
                size = size + ((width >> (i & 0x1f)) * (height >> (i & 0x1f)) * additional >> 3);
            } while (i < mipmaps);
        }
        return size;
    }
    bool Texture::read(BinaryStream& stream)
    {
        auto type = stream.read_word();
        if (!type.has_value()) return false;
        
        auto width = stream.read_word();
        if (!width.has_value()) return false;
        
        auto height = stream.read_word();
        if (!height.has_value()) return false;
        
        auto mipmap_count = stream.read_word();
        if (!mipmap_count.has_value()) return false;

        int mipmap_count_value = mipmap_count.value();
        if (mipmap_count_value == 1) 
            mipmap_count_value = 0;

        int size = texture_size_mm(type.value(), width.value(), height.value(), mipmap_count_value);

        uint8_t *txdata = new uint8_t[size];
        stream.read(size, txdata);
        
        m_type = type.value();
        m_width = width.value();
        m_height = height.value();

        int palette_size = 0;
        if (m_type == 4)
            palette_size = 0x40;
        else if (m_type == 5)
            palette_size = 0x400;

        m_rgba_data.reserve(m_width * m_height * 4);
        if (m_type == 4 || m_type == 5)
        {
            uint8_t *paldata = new uint8_t[palette_size];
            stream.read(palette_size, (uint8_t*) paldata);
            if (m_type == 4)
                size *= 2;
            for (int i = 0; i < size; i++)
            {
                uint8_t idx;
                if (type.value() == 4)
                {
                    if ((i & 1) == 0)
                    {
                        idx = txdata[i >> 1] >> 4;
                    } else {
                        idx = txdata[i >> 1] & 0xf;
                    }
                } else if (type.value() == 5)
                {
                    idx = txdata[i];
                }
                
                uint8_t a = paldata[idx * 4];
                uint8_t r = paldata[idx * 4 + 1];
                uint8_t g = paldata[idx * 4 + 2];
                uint8_t b = paldata[idx * 4 + 3];
                m_rgba_data.push_back(b);
                m_rgba_data.push_back(g);
                m_rgba_data.push_back(r);
                m_rgba_data.push_back(a);
            }
            delete[] paldata;
        } else if (m_type == 3)
        {
            for (int i = 0; i < size; i+=4)
            {
                uint8_t a = txdata[i];
                uint8_t r = txdata[i + 1];
                uint8_t g = txdata[i + 2];
                uint8_t b = txdata[i + 3];
                m_rgba_data.push_back(b);
                m_rgba_data.push_back(g);
                m_rgba_data.push_back(r);
                m_rgba_data.push_back(a);
            }
        } else if (m_type == 2)
        {
            for (int i = 0; i < size; i+=3)
            {
                uint8_t r = txdata[i + 0];
                uint8_t g = txdata[i + 1];
                uint8_t b = txdata[i + 2];
                m_rgba_data.push_back(b);
                m_rgba_data.push_back(g);
                m_rgba_data.push_back(r);
                m_rgba_data.push_back(255);
            }
        }
        delete[] txdata;


        std::cout << "Texture of type " << std::to_string(type.value()) << " of width " << std::to_string(width.value()) << " and height " << std::to_string(height.value()) << ". Mipmaps: " << std::to_string(mipmap_count.value()) << ", palette: " << std::to_string(palette_size) << ", tex size: " << std::to_string(m_rgba_data.size()) << std::endl;
        return true;
    }
    Texture::Texture() {}
}
