#include "material.h"
namespace Nu {
    
    Material::Material() {}
    bool Material::read(BinaryStream& stream)
    {
        stream.seek(SEEK_CUR, 4);
        auto newval = stream.read_word();
        if (!newval.has_value())
            return false;
        
        union { Attributes attr; uint32_t val; } cursed;
        
        // hee hoo bitwise bangin goes brrr
        cursed.val = newval.value();
        m_attributes = cursed.attr;

        if (!m_ambient.read(stream))
            return false;
        if (!m_diffuse.read(stream))
            return false;

        stream.seek(SEEK_CUR, 16);

        auto power_raw = stream.read_float();
        if (!power_raw.has_value()) 
            return false;

        auto alpha_raw = stream.read_float();
        if (!alpha_raw.has_value()) 
            return false;

        auto texture_id = stream.read_word();
        if (!texture_id.has_value())
            return false;

        stream.seek(SEEK_CUR, 24);
        m_power = power_raw.value();
        m_alpha = alpha_raw.value();
        m_texture = texture_id.value();
        return true;
    }

}