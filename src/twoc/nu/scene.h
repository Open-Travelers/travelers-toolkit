#pragma once
#include "../reader_item.h"
#include "twoc/binary_reader.h"
#include "texture.h"
#include "material.h"
#include "instance.h"
#include "spline.h"
#include "vertex.h"
#include "geometry_object.h"
#include <map>

namespace Twoc::Nu
{

enum class SceneType
{
    None,
    Scene,
    GameObject
};

class Scene : public IReaderItem
{
protected:

    SceneType m_type { SceneType::None };
    uint32_t m_block_size[2] { 0, 0 };
    size_t m_block_offset[2] { 0, 0 };

    std::vector<char> m_nametable;
    std::vector<Texture> m_textures;
    std::vector<Material> m_materials;
    std::vector<Instance> m_instances;
    std::vector<Spline> m_splines;
    std::vector<GeometryObject> m_geometry_objects;
    std::map<std::string, size_t> m_spline_lut;
public:
    static Scene *from_reader(Twoc::BinaryReader &reader, size_t fullsize = 0);
    Scene(SceneType type);

    std::string nametable_entry(int i);

    Texture const& texture(int i) { return m_textures[i]; }
    size_t texture_count() const { return m_textures.size(); }

    Material const& material(int i) { return m_materials[i]; }
    size_t material_count() const { return m_materials.size(); }

    Instance const& instance(int i) { return m_instances[i]; }
    size_t instance_count() const { return m_instances.size(); }

    Spline const& spline(int i) { return m_splines[i]; }
    size_t spline_count() const { return m_splines.size(); }

    std::vector<GeometryObject> const& geometry_objects() { return m_geometry_objects; }

    size_t spline_find(std::string const& name)
    {
        if (m_spline_lut.count(name) == 0)
            return -1;

        return m_spline_lut[name];
    }

    size_t size() override { return 0; }
    bool read(Twoc::BinaryReader &reader, size_t fullsize = 0) override;
    bool validate() override;

protected:
    uint32_t begin_block(Twoc::BinaryReader &reader, int depth = 0);
    void end_block(Twoc::BinaryReader &reader, int depth = 0);

    bool read_nametable(Twoc::BinaryReader &reader);
    bool read_texture_set(Twoc::BinaryReader &reader);
    bool read_material_set(Twoc::BinaryReader &reader);
    bool read_geometry_set(Twoc::BinaryReader &reader);
    bool read_instance_set(Twoc::BinaryReader &reader);
    bool read_spline_set(Twoc::BinaryReader &reader);
    bool read_special_objects(Twoc::BinaryReader &reader);
    bool read_animation_library(Twoc::BinaryReader &reader);
};

}

