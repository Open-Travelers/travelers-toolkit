#pragma once 

#include <cstdint>
#include <functional>
#include <memory>
#include "types.h"
#include "window.h"
#include "../app/data.h"

namespace UI {

enum class ViewChange {
    CHANGE,
    WAIT,
    CANCEL
};

class View {
protected:
    friend App::Data;

protected:
    std::function<void(size_t)> change_view;
    std::function<bool(Key)> key_held;

    std::vector<std::shared_ptr<Window>> m_windows;
    int m_display_width, m_display_height;
    App::Data &m_app;

public:
    View(App::Data &app, std::function<bool(Key)> key_held_fn) : m_app(app), key_held(key_held_fn) {}
    virtual ~View() {}

protected:
    virtual void on_load(int display_width, int display_height) = 0;
    virtual ViewChange on_unload() = 0;
    virtual void on_update(float dt) = 0;
    virtual void on_render() = 0;

    virtual void on_resize(int width, int height) {}

    virtual void on_mouse_press(MouseButton button, int x, int y) {}
    virtual void on_mouse_release(MouseButton button, int x, int y) {}
    virtual void on_mouse_moved(int x, int y) {}
    virtual void on_mouse_wheel(bool is_horizontal, float delta) {}

    virtual void on_key_press(Key key, ExtraKeys extra) {}
    virtual void on_key_release(Key key, ExtraKeys extra) {}
    virtual void on_char_write(uint32_t code) {}
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

    void do_load(int display_width, int display_height);
    ViewChange do_unload();

    friend class Manager;
};

}
