
#include "scene.h"

#include <iostream>
#include <set>

namespace Nu {
    bool Scene::read(BinaryStream& stream)
    {
        auto header = stream.read_word();
        if (!header.has_value())
        {
            std::cerr << "End of file before initial header" << std::endl;
            return false;
        }

        // get type of block
        // only GSC0 is supported.
        // this is done through parsing a uint32 so we can get endianness.
        int offset = 0;
        auto header_raw = header.value();
        bool swap_endianness = false;
        if (header_raw == 0x30435347) // GSC0
        {
            std::cout << "Probably a little endian NuScene file." << std::endl;
        } else if (header_raw == 0x47534330) // 0CSG
        {
            std::cerr << "Probably a big endian NuScene file. Unsupported." << std::endl;
            swap_endianness = true;
            return false;
        } else if (header_raw == 0x3032554e) { // NU20
            std::cout << "Probably a little endian compiled NuScene file." << std::endl;
            offset = 8;
        } else if (header_raw == 0x4e553230) { // 02UN
            std::cout << "Probably a big endian compiled NuScene file." << std::endl;
            offset = 8;
            swap_endianness = true;
            return false;
        } else {
            stream.seek(SEEK_SET, 0);
            auto block_name = stream.read_constant_length_string(4);
            std::cerr << "Invalid starting block: " << block_name.value() << std::endl;
            return false;
        }
        // parse size of file value. don't do anything with it, can't guarantee it's correct
        auto value = stream.read_word();
        if (!value.has_value())
        {
            std::cerr << "Premature end of file reached." << std::endl;
            return false;
        }
        if (offset != 0)
            stream.seek(SEEK_CUR, offset);
        m_geometry_definitions.clear();
        m_instances.clear();
        m_materials.clear();
        m_textures.clear();
        m_nametable.clear();

        // start tree parsing
        return read_root(stream);
    }

    bool Scene::read_root(BinaryStream& stream)
    {
        while (!stream.is_eof())
        {
            bool inhibit_correction = false;
            auto starting = stream.tell();
            auto header = stream.read_constant_length_string(4);
            if (!header.has_value())
            {
                std::cerr << "couldnt parse header" << std::endl;
                return false;
            }

            auto size = stream.read_word();
            if (!size.has_value())
                return false;

            auto header_raw = header.value();
            int32_t size_raw = (int32_t) size.value();
            /// ?????
            if (size_raw < 0)
                size_raw = -size_raw;
            std::cout << "Processing header " << header_raw << std::endl;
            if (header_raw == "TST0" || header_raw == "TST2")
            {
                std::cout << "Found texture set of size " << size_raw << std::endl;
                if (!read_texture_set(stream))
                    return false;
            } else if (header_raw == "MS00") {
                auto material_count = stream.read_word();
                if (!material_count.has_value())
                    return false;

                auto material_count_raw = material_count.value();
                for (int i = 0; i < material_count_raw; i++)
                {
                    Material mat;
                    size_t nextpos = stream.tell() + 84;
                    if (!mat.read(stream))
                    {
                        std::cerr << "Material reading failed, skipping." << std::endl;
                        stream.seek(SEEK_SET, nextpos);
                    }
                    m_materials.push_back(mat);
                }
            } else if (header_raw == "NTBL") {
                // skip
                auto str_count = stream.read_word();
                if (!str_count.has_value())
                    return false;
                int used = 4;
                for (int i = 0; i < str_count; i++)
                {
                    auto str = stream.read_zero_terminated_string();
                    if (!str.has_value())
                        return false;
                    m_nametable.push_back(str.value());
                    used += str.value().length() + 1;
                }
                stream.seek(SEEK_CUR, size_raw - 8 - used);
            } else if (header_raw == "GST0") {
                auto count = stream.read_word();
                if (!count.has_value())
                    return false;
                
                m_geometry_definitions.reserve(count.value());
                m_geometry_definitions.clear();
                // alloc count*4
                for (int i = 0; i < count.value(); i++)
                {
                    //std::cout << "Prasing geometry definition " << std::to_string(i) << std::endl;
                    GeometryDefinition definition;
                    if (!definition.read(stream))
                    {
                        std::cerr << "Failed to read geom. definition " << std::to_string(i) << std::endl;
                        return false;
                    }
                    m_geometry_definitions.push_back(definition);

                }
            } else if (header_raw == "INST") {
                auto instance_count = stream.read_word();
                if (!instance_count.has_value())
                    return false;
                m_instances.reserve(instance_count.value());
                m_instances.clear();
                for (int i = 0; i < instance_count.value(); i++)
                {
                    Instance instance;
                    if (!instance.read(stream))
                        return false;
                    m_instances.push_back(instance);
                }
            } else if (header_raw == "SPEC") {
                std::cout << std::to_string(stream.tell()) << std::endl;
                stream.seek(SEEK_CUR, size_raw - 8);
                std::cout << std::to_string(size_raw) << ", " << std::to_string(stream.tell()) << std::endl;
            } else if (header_raw == "SST0") {
                stream.seek(SEEK_CUR, size_raw - 8);
            } else if (header_raw == "ALIB") {
                stream.seek(SEEK_CUR, size_raw - 8);
            } else if (header_raw == "OBJ0") {
                stream.seek(SEEK_CUR, size_raw - 8);
            } else if (header_raw == "IABL") {
                stream.seek(SEEK_CUR, size_raw - 8);
            } else if (header_raw == "TAS0") {
                stream.seek(SEEK_CUR, size_raw - 8);
            } else {
                std::cerr << "Unknown root header '" << header_raw << "'" << std::endl;
                return false;
            }

            auto ending = stream.tell();
            if ((ending - starting < size_raw))
                stream.seek(SEEK_CUR, size_raw - (ending - starting));
        }

        std::set<int> bruh;
        for (auto& instance : m_instances)
            bruh.insert(instance.get_geometry_object());
        
        bool unused = false;
        for (int i = 0; i < m_geometry_definitions.size(); i++)
        {
            if (bruh.find(i) == bruh.end())
            {
                std::cout << "Found unused geometry definition: " << std::to_string(i) << std::endl;
                unused = true;
            }
        }

        if (!unused)
        {
            std::cout << "Scene contains no unused geometry definitions." << std::endl;
        }

        struct bounds {
            Vec3 min;
            Vec3 max;
        };
        struct bounds world_bounds = { Vec3(0, 0, 0), Vec3(0, 0, 0)};
        std::vector<struct bounds> geom_def_bounds;
        for (auto geomdef : m_geometry_definitions)
        {
            Vec3 min(0, 0, 0), max(0, 0, 0);
            for (auto part : geomdef.get_parts())
            {
                for (auto vtx : part.get_vertices())
                {
                    Vec3 pos = vtx.position();
                    if (pos.x() < min.x())
                        min.set_x(pos.x());
                    if (pos.y() < min.y())
                        min.set_y(pos.y());
                    if (pos.z() < min.z())
                        min.set_z(pos.z());
                    if (pos.x() > max.x())
                        max.set_x(pos.x());
                    if (pos.y() > max.y())
                        max.set_y(pos.y());
                    if (pos.z() > max.z())
                        max.set_z(pos.z());
                }
            }
            geom_def_bounds.push_back((struct bounds) { min, max });
        }

        for (auto& instance : m_instances)
        {
            int id = instance.get_geometry_object();
            if (id < 0 || id >= geom_def_bounds.size())
                continue;
            
            struct bounds local_bounds = geom_def_bounds[id];
            local_bounds.min = instance.get_transform_matrix().transform_vector(local_bounds.min);
            local_bounds.max = instance.get_transform_matrix().transform_vector(local_bounds.max);
            if (world_bounds.min.x() > local_bounds.min.x())
                world_bounds.min.set_x(local_bounds.min.x());
            if (world_bounds.min.y() > local_bounds.min.y())
                world_bounds.min.set_y(local_bounds.min.y());
            if (world_bounds.min.z() > local_bounds.min.z())
                world_bounds.min.set_z(local_bounds.min.z());

            if (world_bounds.max.x() < local_bounds.max.x())
                world_bounds.max.set_x(local_bounds.max.x());
            if (world_bounds.max.y() < local_bounds.max.y())
                world_bounds.max.set_y(local_bounds.max.y());
            if (world_bounds.max.z() < local_bounds.max.z())
                world_bounds.max.set_z(local_bounds.max.z());
        }
        std::cout << "Parsed " << m_geometry_definitions.size() << " geometry definitions" << std::endl;
        std::cout << "Parsed " << m_instances.size() << " instances" << std::endl;
        std::cout << "Parsed " << m_textures.size() << " textures" << std::endl;
        m_min_bounds = world_bounds.min;
        m_max_bounds = world_bounds.max;
        return true;
    }

    bool Scene::read_texture_set(BinaryStream& stream)
    {
        long int texture_count = -1;
        while (!stream.is_eof())
        {
            auto header = stream.read_constant_length_string(4);
            if (!header.has_value())
                return false;

            auto size = stream.read_word();
            if (!size.has_value())
                return false;

            auto header_raw = header.value();
            auto size_raw = size.value();
            if (header_raw == "TSH0")
            {
                auto read_texture_count = stream.read_word();
                if (!read_texture_count.has_value())
                    return false;

                texture_count = read_texture_count.value();
                std::cout << "Found texture set header with " << std::to_string(texture_count) << " textures" << std::endl;
            } else if (header_raw == "TXM0") {
                if (texture_count < 0) {
                    std::cerr << "Tried to read texture without texture set header" << std::endl;
                }

                Texture texture;
                size_t next_pos = stream.tell() - 8 + size_raw;
                if (!texture.read(stream))
                {
                    std::cerr << "Failed to parse texture, skipping." << std::endl;
                    stream.seek(SEEK_SET, next_pos);
                } else {
                    m_textures.push_back(texture);
                }

                texture_count--;
                if (texture_count == 0)
                    return true;
            } else {
                std::cout << "Unknown texture set header '" << header_raw << "'" << std::endl;
                stream.seek(SEEK_CUR, -8);
                break;
            }
        }

        return true;
    }

}