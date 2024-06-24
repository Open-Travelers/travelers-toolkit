#include "texture.h"
#include <iostream>
namespace Nu {
    static const int texture_size(int type, int width, int height, int pixlength)
    {
        static constexpr int sizes[] = { 0x10, 0x10, 0x18, 0x20, 4, 8, 0 };
        
        int size = width * height;
        if (type >= 0 && type <= 5)
            size *= (sizes[type] >> 3);
        else size = 0;

        for (int i = 0; i < pixlength; i++)
        {
            int additional = sizes[type];
            size += (width >> (i & 0x1f)) * (height >> (i & 0x1f)) * (additional >> 3);
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

        int size = texture_size(type.value(), width.value(), height.value(), mipmap_count_value);
        stream.seek(SEEK_CUR, size);
        
        int palette_size = 0;
        if (type.value() == 4)
            palette_size = 0x40;
        else if (type.value() == 5)
            palette_size = 0x400;

        if (palette_size != 0)
            stream.seek(SEEK_CUR, palette_size);

        std::cout << "Texture of type " << std::to_string(type.value()) << " of width " << std::to_string(width.value()) << " and height " << std::to_string(height.value()) << ". Mipmaps: " << std::to_string(mipmap_count.value()) << std::endl;
        return true;
    }

}