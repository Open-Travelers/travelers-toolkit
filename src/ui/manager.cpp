
#include <iostream>
#include "manager.h"

namespace UI {

    Manager::Manager()
    {

    }
    
    void Manager::handle_view_pending_change()
    {
        auto old_view = get_view(m_view_current);
        if (old_view) 
        {
            // exit early if the current view isn't ready to change
            // or if it's decided to cancel the operation
            auto result = old_view->do_unload();
            if (result == ViewChange::WAIT)
            {
                return;
            } else if (result == ViewChange::CANCEL)
            {
                m_view_pending = -1;
                m_view_pending_change = false;
                return;
            }
        }

        auto view = get_view(m_view_pending);
        if (view) {
            view->do_load(m_display_width, m_display_height);
        }

        std::cout << "View changed to " << m_view_pending << std::endl;
        m_view_current = m_view_pending;
        m_view_pending_change = false;
    }

    void Manager::add_view(std::shared_ptr<View> view)
    {
        view->change_view = [&](size_t view) { change_view(view); };
        m_views.emplace_back((view));
    }
    std::shared_ptr<View> Manager::get_view(size_t index)
    {
        if (index > m_views.size())
            return {};

        return (m_views[index]);
    }

    void Manager::change_view(size_t view)
    {
        m_view_pending_change = true;
        m_view_pending = view;
        handle_view_pending_change();
    }

    void Manager::do_update(float dt) 
    {
        auto view = get_view(m_view_current);
        if (!view)
            return;

        view->do_update(dt);
        if (m_view_pending_change) {
            handle_view_pending_change();
        }
    }
    void Manager::do_render() 
    {
        auto view = get_view(m_view_current);
        if (!view)
            return;
        view->do_render();
    }

    void Manager::do_resize(int width, int height) 
    {
        m_display_width = width;
        m_display_height = height;
        auto view = get_view(m_view_current);
        if (!view)
            return;
        view->do_resize(width, height);
    }

    void Manager::do_mouse_press(MouseButton button, int x, int y) 
    {
        auto view = get_view(m_view_current);
        if (!view)
            return;
        view->do_mouse_press(button, x, y);
    }
    void Manager::do_mouse_release(MouseButton button, int x, int y) 
    {
        auto view = get_view(m_view_current);
        if (!view)
            return;
        view->do_mouse_release(button, x, y);
    }
    void Manager::do_mouse_moved(int x, int y) 
    {
        auto view = get_view(m_view_current);
        if (!view)
            return;
        view->do_mouse_moved(x, y);
    }

    void Manager::do_key_press(Key key, ExtraKeys extra) 
    {
        auto view = get_view(m_view_current);
        if (!view)
            return;
        view->do_key_press(key, extra);
    }
    void Manager::do_key_release(Key key, ExtraKeys extra)
    {
        auto view = get_view(m_view_current);
        if (!view)
            return;
        view->do_key_release(key, extra);
    }
    void Manager::do_char_write(uint32_t code)
    {
        auto view = get_view(m_view_current);
        if (!view)
            return;
        view->do_char_write(code);
    }
}
