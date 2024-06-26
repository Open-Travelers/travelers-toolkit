#ifndef TEXTURE_H
#define TEXTURE_H

#include "../binary_stream.h"
namespace Nu {

    class Texture
    {
    public:
        Texture();
        bool read(BinaryStream& stream);
        uint8_t *get_rgba_data() { return m_rgba_data.data(); }
        int get_rgba_data_size() { return m_rgba_data.size(); }
        int width() const { return m_width; }
        int height() const { return m_height; }
    protected:
        int m_type { 0 };
        int m_width { 0 };
        int m_height { 0 };
        std::vector<uint8_t> m_rgba_data;
    };

}
#endif
