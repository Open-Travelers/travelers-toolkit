#pragma once

#include <SFML/Graphics.hpp>
#include "ui/manager.h"
#include "application_data.h"

class Application {
protected:
    UI::Manager m_ui {};
    ApplicationData m_data {};
public:
    int run(int argc, char *argv[]);

public:
    Application();
    ~Application();

};
