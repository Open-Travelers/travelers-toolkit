#include "material.h"
namespace Nu {

    bool Material::read(BinaryStream& stream)
    {
        stream.seek(SEEK_CUR, 20);
        auto diffuse_r = stream.read_float();
        if (!diffuse_r.has_value())
            return false;
        auto diffuse_g = stream.read_float();
        if (!diffuse_r.has_value())
            return false;
        auto diffuse_b = stream.read_float();
        if (!diffuse_r.has_value())
            return false;

        stream.seek(SEEK_CUR, 24);

        auto texture_id = stream.read_word();
        if (!texture_id.has_value())
            return false;

        stream.seek(SEEK_CUR, 24);
        m_diffuse_r = diffuse_r.value();
        m_diffuse_g = diffuse_g.value();
        m_diffuse_b = diffuse_b.value();
        m_texture = texture_id.value();
        return true;
    }

}