#ifndef NU_SCENE_H
#define NU_SCENE_H
#include "../binary_stream.h"
#include "texture.h"
#include "geometry_definition.h"
#include "instance.h"
#include "material.h"

namespace Nu {

    class Scene {
    public:
        bool read(BinaryStream& stream);

        std::vector<GeometryDefinition> const& get_definitions() { return m_geometry_definitions; }
        std::vector<Instance> const& get_instances() { return m_instances; }
        std::vector<Texture> const& get_textures() { return m_textures; }

        Material const* get_materials_raw() { return m_materials.data(); }
        GeometryDefinition const* get_definitions_raw() { return m_geometry_definitions.data(); }
        int get_definition_count() { return m_geometry_definitions.size(); }
        Vec3 min_bounds() { return m_min_bounds; }
        Vec3 max_bounds() { return m_max_bounds; }
    private:
        bool read_root(BinaryStream& stream);
        bool read_texture_set(BinaryStream& stream);
    protected:
        std::vector<GeometryDefinition> m_geometry_definitions;
        std::vector<Instance> m_instances;
        std::vector<Texture> m_textures;
        std::vector<Material> m_materials;
        Vec3 m_min_bounds { Vec3(0, 0, 0) };
        Vec3 m_max_bounds { Vec3(0, 0, 0) };
    };

}
#endif
