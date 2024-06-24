#ifndef TEXTURE_H
#define TEXTURE_H

#include "../binary_stream.h"
namespace Nu {

    class Texture
    {
    public:
        bool read(BinaryStream& stream);
    protected:
        int m_width { 0 };
        int m_height { 0 };
    };

}
#endif
