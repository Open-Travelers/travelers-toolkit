#include "geometry_part.h"

#include <iostream>

namespace Nu {
    GeometryPart::GeometryPart()
    {

    }
    bool GeometryPart::read_vertices(BinaryStream &stream)
    {
        auto vtx_count = stream.read_word();
        if (!vtx_count.has_value()) return false;

        //std::cout << "Vertex count: " << std::to_string(vtx_count.value()) << std::endl;
        m_vertices.reserve(vtx_count.value());
        for (int i = 0; i < vtx_count.value(); i++)
        {
            Vertex vtx;
            if (!vtx.read(stream))
                return false;
            m_vertices.push_back(vtx);
        }
        return true;
    }
    bool GeometryPart::read_primitives(BinaryStream &stream)
    {
        auto prim_count = stream.read_word();
        if (!prim_count.has_value()) return false;

        m_primitives.reserve(prim_count.value());
        m_primitives.clear();
        for (int prim_id = 0; prim_id < prim_count.value(); prim_id++)
        {
            GeometryPrimitive primitive;
            if (!primitive.read(stream))
            {
                std::cerr << "Failed to read primitive " << std::to_string(prim_id) << std::endl;
                return false;
            }
            m_primitives.push_back(primitive);
        }
        return true;
    }
    bool GeometryPart::read_control(BinaryStream &stream)
    {
        auto control = stream.read_word();
        if (!control.has_value()) return false;
        //std::cout << "Control word: " << std::to_string(control.value()) << std::endl;
        if (control.value() != 0)
            stream.seek(SEEK_CUR, control.value() * 0xc);
        return true;
    }
    bool GeometryPart::read_skins(BinaryStream &stream)
    {
        auto skin_count = stream.read_word();
        if (!skin_count.has_value()) return false;

        if (skin_count.value() != 0)
        {
            auto some_byte = stream.read_byte();
            if (!some_byte.has_value()) return false;

            if (some_byte.value() == 0)
            {

                for (int i = 0; i < skin_count.value(); i++)
                {
                    auto some_int = stream.read_word();
                    if (!some_int.has_value()) return false;
                    
                    auto some_int_2 = stream.read_word();
                    if (!some_int_2.has_value()) return false;

                    auto some_count = stream.read_word();
                    if (!some_count.has_value()) return false;

                    for (int j = 0; j < some_count.value(); j++)
                    {
                        auto some_int_j = stream.read_word();
                        if (!some_int_j.has_value()) return false;
                    }

                    stream.seek(SEEK_CUR, some_int_2.value() * some_count.value() * 4);
                }
            } else {
                stream.seek(SEEK_CUR, m_vertices.size() << 4);
            }
        }
        return true;
    }
    bool GeometryPart::read_blendshapes(BinaryStream &stream)
    {
        auto count = stream.read_word();
        if (!count.has_value()) return false;
        //std::cout << "Blendshape count: " << count.value() << std::endl;
        if (count.value() != 0)
        {
            stream.seek(SEEK_CUR, count.value() << 2);
            auto some_count = stream.read_word();
            if (!some_count.has_value()) 
                return false;
            stream.seek(SEEK_CUR, some_count.value());
            for (int i = 0; i < count.value(); i++)
            {
                auto v = stream.read_byte();
                if (!v.has_value()) return false;
            }
        }
        return true;
    }
    bool GeometryPart::read(BinaryStream &stream)
    {
        stream.seek(SEEK_CUR, 4);
        // regular geometry object
        // GeomVtx
        //std::cout << "Reading part vertices..." << std::endl;
        if (!read_vertices(stream))
        {
            std::cerr << "Failed to read vertices" << std::endl;
            return false;
        }

        // GeomCntrl
        //std::cout << "Reading part control..." << std::endl;
        if (!read_control(stream))
        {
            std::cerr << "Failed to read control" << std::endl;
            return false;
        }

        // GeomPrim
        //std::cout << "Reading part primitives..." << std::endl;
        if (!read_primitives(stream))
        {
            std::cerr << "Failed to read primitives" << std::endl;
            return false;
        }

        // GeomSkin 
        //std::cout << "Reading part skins..." << std::endl;
        if (!read_skins(stream))
        {
            std::cerr << "Failed to read skins" << std::endl;
            return false;
        }

        // BlendShape
        //std::cout << "Reading part blendshapes..." << std::endl;
        if (!read_blendshapes(stream))
        {
            std::cerr << "Failed to read blendshapes" << std::endl;
            return false;
        }
        
        return true;
    }
}