#include "geometry_definition.h"

#include <iostream>
namespace Nu {
    GeometryDefinition::GeometryDefinition() 
    {

    }
    bool GeometryDefinition::read(BinaryStream &stream)
    {
        auto entry_count = stream.read_word();
        if (!entry_count.has_value())
            return false;
        
        //std::cout << "Definition entry count is " << std::to_string(entry_count.value()) << std::endl;
        for (int i = 0; i < entry_count.value(); i++)
        {
            auto is_face = stream.read_word();
            if (!is_face.has_value())
                return false;
            if (is_face.value() == 0)
            {
                stream.seek(SEEK_CUR, 0xc);
                auto geometry_object_count = stream.read_word();
                if (!geometry_object_count.has_value())
                    return false;
                
                m_parts.reserve(geometry_object_count.value());
                m_parts.clear();
                for (int j = 0; j < geometry_object_count.value(); j++)
                {
                    GeometryPart part;
                    if (!part.read(stream))
                    {
                        std::cerr << "Failed to read geometry part " << std::to_string(j) << std::endl;
                        return false;
                    }
                    m_parts.push_back(part);
                }
            } else {
                /// TODO: Figure out what faces actually do
                auto face_count = stream.read_word();
                if (!face_count.has_value())
                    return false;

                for (int face_id = 0; face_id < face_count.value(); face_id++)
                {
                    auto some_int = stream.read_word();
                    if (!some_int.has_value()) return false;

                    auto some_data_count = stream.read_word();
                    if (!some_data_count.has_value()) return false;

                    auto some_int_3 = stream.read_word();
                    if (!some_int_3.has_value()) return false;

                    auto some_float = stream.read_float();
                    if (!some_float.has_value()) return false;

                    stream.seek(SEEK_CUR, some_data_count.value() * 0x18);
                }
            }
            
        }
        //std::cout << std::to_string(stream.tell()) << std::endl;
        return true;
    }
}