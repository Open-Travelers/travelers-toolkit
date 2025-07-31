#pragma once
#include <string>

namespace App {

enum class ActionType : short {
    LOAD_LEVEL,
};

struct Action {
    ActionType Type;
    union {
        struct { const char *Name; } Level;
    } u;

    ~Action() = default;
};


}