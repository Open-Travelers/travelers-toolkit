#pragma once

#include <SFML/Graphics.hpp>
#include "../ui/manager.h"
#include "action.h"
#include "data.h"
#include <queue>

namespace App {

class Application {
protected:
    UI::Manager m_ui {};
    Data m_data {};
    std::queue<Action> m_action_queue;

public:
    int run(int argc, char *argv[]);
    void queue_action(Action action);

public:
    Application();
    ~Application();

};

}
