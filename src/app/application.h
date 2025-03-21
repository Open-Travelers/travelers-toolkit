#pragma once

#include <SFML/Graphics.hpp>
#include "../ui/manager.h"
#include "data.h"

namespace App {

class Application {
protected:
    UI::Manager m_ui {};
    Data m_data {};
public:
    int run(int argc, char *argv[]);

public:
    Application();
    ~Application();

};

}
