#pragma once

#include <stdint.h>
#include <memory>
#include "types.h"
#include "view.h"

namespace UI {

class Manager {
protected:
    std::vector<std::shared_ptr<View>> m_views;
    size_t m_view_current { -1u };

    size_t m_view_pending { -1u };
    bool m_view_pending_change { false };
    std::shared_ptr<View> get_view(size_t index);

    int m_display_width;
    int m_display_height;

public:
    Manager();

public:
    void add_view(std::shared_ptr<View> view);
    void change_view(size_t view);

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

protected:
    void handle_view_pending_change();
};

}
