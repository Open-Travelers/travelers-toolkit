#pragma once

namespace UI {

enum class WindowState {
    OPENED = 0,
    CLOSING = 1
};

class Window 
{
public:

protected:
    WindowState m_state { WindowState::OPENED };

public:
    WindowState state() { return m_state; }
    void set_state(WindowState state) { m_state = state; }

public:
    virtual void do_draw();
    virtual ~Window();
};

}