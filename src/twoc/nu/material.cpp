#include "material.h" 
#include "../reader_item.h"

namespace Twoc::Nu 
{

ITEM_DEFINE(Material, MATERIAL_ITEM)

bool Material::validate()
{
    return true;
}

}
