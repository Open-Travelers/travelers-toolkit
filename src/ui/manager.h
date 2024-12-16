#ifndef MANAGER_H
#define MANAGER_H

#include <stdint.h>
#include "types.h"

namespace UI {

class Manager {
public:
    Manager();

public:
    void do_update(float dt);
    void do_render();

    void do_resize(int width, int height);

    void do_mouse_press(MouseButton button, int x, int y);
    void do_mouse_release(MouseButton button, int x, int y);
    void do_mouse_moved(int x, int y);
    void do_mouse_wheel(bool is_horizontal, float delta);

    void do_key_press(Key key, ExtraKeys extra);
    void do_key_release(Key key, ExtraKeys extra);
    void do_char_write(uint32_t code);
};

}

#endif
