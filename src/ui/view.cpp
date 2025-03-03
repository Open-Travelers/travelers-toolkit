#include "view.h"

namespace UI {

void View::do_update(float dt)
{
    on_update(dt);
}
void View::do_render()
{
    on_render();

    auto it = m_windows.begin();
    while (it != m_windows.end())
    {
        auto window = *it;
        window->do_draw();
        if (window->state() == WindowState::CLOSING)
            it = m_windows.erase(it);
    }
}

void View::do_resize(int width, int height)
{
    m_display_width = width;
    m_display_height = height;
    on_resize(width, height);
}

void View::do_mouse_press(MouseButton button, int x, int y)
{
    on_mouse_press(button, x, y);
}
void View::do_mouse_release(MouseButton button, int x, int y)
{
    on_mouse_release(button, x, y);
}
void View::do_mouse_moved(int x, int y)
{
    on_mouse_moved(x, y);
}
void View::do_mouse_wheel(bool is_horizontal, float delta)
{
    on_mouse_wheel(is_horizontal, delta);
}

void View::do_key_press(Key key, ExtraKeys extra)
{
    on_key_press(key, extra);
}
void View::do_key_release(Key key, ExtraKeys extra)
{
    on_key_release(key, extra);
}
void View::do_char_write(uint32_t code)
{
    on_char_write(code);
}


void View::do_load(int display_width, int display_height)
{
    m_display_width = display_width;
    m_display_height = display_height;
    on_load(display_width, display_height);
}
ViewChange View::do_unload()
{
    return on_unload();
}

}