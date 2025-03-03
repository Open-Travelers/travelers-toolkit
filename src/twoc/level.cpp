#include "level.h"

namespace Twoc
{

Level::Level(LevelData *data, Nu::Scene *scene, Crate::CrateStore *crate_store) : m_data(data), m_scene(scene), m_crate_store(crate_store) {}
Level::~Level() { }

}
