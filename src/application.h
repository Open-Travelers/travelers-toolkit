
#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.hpp>
#include "ui/manager.h"


class Application {
protected:
    UI::Manager m_ui;

public:
    int run(int argc, char *argv[]);

public:
    Application();
    ~Application();

};


#endif
