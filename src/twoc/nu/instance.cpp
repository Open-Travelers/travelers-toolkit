
#include "instance.h"
#include <glm/gtc/type_ptr.hpp>

namespace Twoc::Nu 
{
    
ITEM_DEFINE(InstanceRaw, INSTANCE_ITEM)

bool InstanceRaw::validate()
{
    return true;
}

Instance::Instance(InstanceRaw raw)
{
    m_transform_matrix = glm::make_mat4(raw.data().transform_matrix);
    m_object_index = raw.object_index();
    m_flags = (Twoc::Nu::InstanceFlags) raw.flags();
    m_room_group = raw.room_group();
    m_special_flag = raw.special_flag();
}

}
