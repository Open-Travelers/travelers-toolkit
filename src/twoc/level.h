#ifndef LEVEL_H
#define LEVEL_H

#include "level_data.h"
#include "nu/scene.h"
#include "crt/crate_store.h"

namespace Twoc
{

class Level
{
protected:
    LevelData *m_data;
    Nu::Scene *m_scene;
    Crate::CrateStore *m_crate_store;

public:
    Level(LevelData *data, Nu::Scene *scene, Crate::CrateStore *crate_store);
    ~Level();

    LevelData *level_data() const { return m_data; }
    Nu::Scene *scene() const { return m_scene; }
    Crate::CrateStore *crate_store() const { return m_crate_store; }
};

}
#endif // LEVEL_H
