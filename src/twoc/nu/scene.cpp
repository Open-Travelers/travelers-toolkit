#include "scene.h"
#include "texture.h"
#include "twoc/nu/vertex.h"
#include "twoc/nu/primitive.h"
#include <glm/vec3.hpp>

#define BLK_CSG0 (0x30435347)
#define BLK_NTBL (0x4C42544E)
#define BLK_TST0 (0x30545354)
#define BLK_TSH0 (0x30485354)
#define BLK_TXM0 (0x304D5854)
#define BLK_MS00 (0x3030534D)
#define BLK_GST0 (0x30545347)
#define BLK_INST (0x54534E49)
#define BLK_SST0 (0x30545353)
#define BLK_SPEC (0x43455053)
#define BLK_ALIB (0x42494C41)
#define BLK_TAS0 (0x30534154)
#define BLK_LDIR (0x5249444c)
#define BLK_SPHE (0x45485053)
namespace Twoc::Nu
{

Scene::Scene(SceneType type) : m_type(type)
{

}

std::string Scene::nametable_entry(int i)
{
    if (i < 0 || i > m_nametable.size())
        return "<N/A>";

    auto it = m_nametable.begin() + i;
    std::string str = "";

    while (*it != 0 && it != m_nametable.end())
    {
        str += *it;
        it++;
    }

    return str;
}

uint32_t Scene::begin_block(Twoc::BinaryReader &reader, int depth)
{
    size_t current = reader.tell();

    uint32_t type = reader.u32();
    uint32_t size = reader.u32();

    if (reader.status() != Twoc::ReaderStatus::Ok)
        return 0;
    union {
        char text[8];
        uint64_t value;
    } u;
    u.value = type;
    std::cout << depth << ": " << u.text << std::endl;

    m_block_offset[depth] = current;
    m_block_size[depth] = size;
    return type;
}

void Scene::end_block(Twoc::BinaryReader &reader, int depth)
{
    ssize_t offset = m_block_size[depth] - (reader.tell() - m_block_offset[depth]);
    if (offset > 0)
        reader.seek(ReaderBase::Current, offset);
}

bool Scene::read_nametable(Twoc::BinaryReader &reader)
{
    std::vector<char> nametable;

    uint32_t byte_count = reader.u32();
    nametable = reader.array<char>(byte_count);

    m_nametable = nametable;
    return reader.status() != Twoc::ReaderStatus::Error;
}

bool Scene::read_texture_set(Twoc::BinaryReader &reader)
{
    uint32_t texture_count = 0;
    std::vector<Texture> textures;
    uint32_t block = 0;
    for (;;)
    {
        if ((block != 0) && (texture_count == 0))
        {
            m_block_size[0] = 0;
            m_block_offset[0] = reader.tell();
            m_textures = textures;
            return reader.status() != Twoc::ReaderStatus::Error;
        }

        block = begin_block(reader, 1);
        if (!block)
            return false;

        switch (block)
        {
        case BLK_TSH0:
            texture_count = reader.u32();
            end_block(reader, 1);
            continue;

        case BLK_TXM0: {
            if (texture_count == 0)
                return false;

            Texture tex;
            if (!tex.read(reader))
                return false;

            textures.emplace_back(tex);
            end_block(reader, 1);
            texture_count--;
        } continue;

        default:
            std::cerr << "Unknown level-2 header: " << block << std::endl;
            return false;
        }
    }
    m_block_size[0] = 0;
    m_block_offset[0] = reader.tell();
    m_textures = textures;
    return reader.status() != Twoc::ReaderStatus::Error;
}

bool Scene::read_texture_animation_set(Twoc::BinaryReader &reader)
{
    uint32_t texture_animations = reader.u32();
    (void) reader.u32();
    (void) reader.seek(ReaderBase::Current, texture_animations * 32);

    uint32_t texture_id_count = reader.u32();
    std::vector<uint16_t> texture_ids = reader.array<uint16_t>(texture_id_count);
    (void) texture_ids;
    return true;
}

bool Scene::read_material_set(Twoc::BinaryReader &reader)
{
    std::vector<Nu::Material> materials;

    uint32_t count = reader.u32();
    for (int i = 0; i < count; i++)
    {
        Nu::Material material;
        if (!material.read(reader))
        {
            std::cerr << "Failed to read material " << i << std::endl;
            return false;
        }
        materials.push_back(material);
    }

    m_materials = materials;
    return reader.status() != Twoc::ReaderStatus::Error;
}

bool Scene::read_geometry_set(Twoc::BinaryReader &reader)
{
    std::vector<GeometryObject> geometry_objects;
    uint32_t count = reader.u32();
    for (int i = 0; i < count; i++)
    {
        uint32_t object_count = reader.u32();
        for (int j = 0; j < object_count; j++)
        {
            uint32_t object_type = reader.u32();
            if (object_type == 0)
            {
                GeometryObject gobj;
                if (!gobj.read(reader))
                {
                    std::cerr << "Failed to read geometry object " << j << std::endl;
                    return false;
                }
                geometry_objects.push_back(gobj);
            } else {
                // FaceOn?
                uint32_t face_count = reader.u32();
                for (int z = 0; z < face_count; z++)
                {
                    uint32_t unk1 = reader.u32();
                    uint32_t data_count = reader.u32();
                    uint32_t unk2 = reader.u32();
                    float unk3 = reader.f32();
                    reader.seek(ReaderBase::Current, data_count * 0x18);
                }

                GeometryObject empty_obj;
                geometry_objects.push_back(empty_obj);
            }
        }
    }
    m_geometry_objects = geometry_objects;
    return reader.status() != Twoc::ReaderStatus::Error;
}

bool Scene::read_instance_set(Twoc::BinaryReader &reader)
{
    uint32_t count = reader.u32();

    std::vector<Instance> instances;
    for (int i = 0; i < count; i++)
    {
        InstanceRaw instance;
        if (!instance.read(reader))
        {
            std::cerr << "Failed to read instance " << i << std::endl;
            return false;
        }
        instances.push_back(Instance(instance));
    }

    count = reader.u32();
    for (int i = 0; i < count; i++)
    {
        reader.seek(Twoc::ReaderBase::Current, 0x60);
    }
    m_instances = instances;
    return true;
}

bool Scene::read_spline_set(Twoc::BinaryReader &reader)
{
    uint32_t count = reader.u32();
    uint32_t spline_name_index = reader.u32();
    std::vector<Spline> splines;

    for (int i = 0; i < count; i++)
    {
        uint16_t length, pad1;
        uint16_t nametable_index, pad2;
        if (reader.file_endianness() == Twoc::ReaderEndianness::Little)
        {
            length = reader.u16();
            pad1 = reader.u16();
            nametable_index = reader.u16();
            pad2 = reader.u16();
        } else {
            pad1 = reader.u16();
            length = reader.u16();
            pad2 = reader.u16();
            nametable_index = reader.u16();
        }

        std::vector<glm::vec3> points;
        for (uint16_t j = 0; j < length; j++)
        {
            float x = reader.f32();
            float y = reader.f32();
            float z = reader.f32();
            points.emplace_back(x, y, z);
        }

        splines.push_back(Spline(nametable_index, points));
    }

    m_splines = splines;
    return true;
}

bool Scene::read_special_objects(Twoc::BinaryReader &reader)
{
    return true;
}

bool Scene::read_animation_library(Twoc::BinaryReader &reader)
{
    return true;
}

bool Scene::read(Twoc::BinaryReader &reader, size_t fullsize)
{
    uint32_t block = begin_block(reader);
    switch (block)
    {
    case BLK_CSG0:
        break;
    default:
        std::cerr << "Unknown root header: " << std::to_string(block) << "!" << std::endl;
        return false;
    }

    for (;;)
    {
        block = begin_block(reader);
        if (block == 0)
            break;

        bool result = false;
        switch (block)
        {
        case BLK_NTBL:
            result = read_nametable(reader);
            break;
        case BLK_TST0:
            result = read_texture_set(reader);
            break;
        case BLK_MS00:
            result = read_material_set(reader);
            break;
        case BLK_GST0:
            result = read_geometry_set(reader);
            break;
        case BLK_INST:
            result = read_instance_set(reader);
            break;
        case BLK_SST0:
            result = read_spline_set(reader);
            break;
        case BLK_SPEC:
            result = read_special_objects(reader);
            break;
        case BLK_ALIB:
            result = read_animation_library(reader);
            break;
        case BLK_TAS0:
            result = read_texture_animation_set(reader);
            break;
        case BLK_LDIR:
            result = true;
            break;
        case BLK_SPHE:
            result = true;
            break;
        default:
            std::cerr << "Unknown level-1 header: " << std::to_string(block) << "!" << std::endl;
            return false;
        }

        if (!result)
        {
            std::cerr << "Error parsing... Something." << std::endl;
            return false;
        }
        end_block(reader);
    }

    size_t i = 0;
    for (auto &spline : m_splines)
    {
        auto name = nametable_entry(spline.m_name_index);
        spline.update_name(name);
        m_spline_lut[name] = i;
        i++;
    }

    return true;
}

Scene *Scene::from_reader(Twoc::BinaryReader &reader, size_t fullsize)
{
    Scene *result = new Scene(SceneType::None);
    if (!result)
        return NULL;

    if (!result->read(reader, fullsize))
    {
            delete result;
            return NULL;
    }
    return result;
}

bool Scene::validate()
{
    return false;
}
}
