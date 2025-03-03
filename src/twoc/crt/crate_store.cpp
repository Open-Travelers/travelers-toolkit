
#include "crate_store.h"
#include "glm/glm.hpp"

namespace Twoc::Crate
{

CrateStore::~CrateStore()
{


}
CrateStore *CrateStore::from_reader(BinaryReader &reader)
{
    CrateStore *store = new CrateStore();
    if (!store)
        return nullptr;

    if (!store->read(reader))
    {
        delete store;
        return nullptr;
    }
    return store;
}

bool CrateStore::read(BinaryReader &reader)
{
    uint32_t version = reader.u32();
    uint16_t crate_group_count = reader.u16();
    std::vector<Crate> crates;
    std::vector<CrateGroup> groups;

    for (int i = 0; i < crate_group_count; i++)
    {
        glm::vec3 group_position;
        group_position.x = reader.f32();
        group_position.y = reader.f32();
        group_position.z = reader.f32();

        uint16_t crate_offset = reader.u16();
        uint16_t crate_count = reader.u16();
        uint16_t tilt = reader.u16();

        std::vector<uint16_t> indices;
        std::vector<Crate> crates;

        for (int j = 0; j < crate_count; j++)
        {
            glm::vec3 crate_position;
            crate_position.x = reader.f32();
            crate_position.y = reader.f32();
            crate_position.z = reader.f32();
            float unk1 = reader.f32();

            uint16_t local_x, local_y, local_z;
            local_x = reader.u16();
            local_y = reader.u16();
            local_z = reader.u16();

            CrateType crate_types[4] = { CrateType::None, CrateType::None, CrateType::None, CrateType::None };
            crate_types[0] = (CrateType) reader.s8();
            if (version >= 3)
            {
                crate_types[1] = (CrateType) reader.s8();
                crate_types[2] = (CrateType) reader.s8();
                crate_types[3] = (CrateType) reader.s8();
            }

            int16_t unk2, unk3, unk4, unk5, unk6, unk7;
            unk2 = reader.s16();
            unk3 = reader.s16();
            unk4 = reader.s16();
            unk5 = reader.s16();
            unk6 = reader.s16();
            unk7 = reader.s16();
            int16_t exclamation_crate_index = reader.s16();

            int16_t unk_array[] = { unk2, unk3, unk4, unk5, unk6, unk7 };
            crates.emplace_back(Crate(crate_position, local_x, local_y, local_z, crate_types, exclamation_crate_index, unk1, unk_array));
        }

        groups.emplace_back(CrateGroup(group_position, tilt, crate_offset, indices));
    }

    return true;
}

}
