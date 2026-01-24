extern "C" {
#include <glad/glad.h>
#include <GLFW/glfw3.h>
}

#include <iostream>
#include <imgui.h>
#include <portable-file-dialogs.h>
#include <filesystem>

#include "../imgui_impl_glfw.h"
#include "../imgui_impl_opengl3.h"

#include "../views/welcome/welcome_view.h"
#include "application.h"

namespace App {

static GLFWwindow *g_window = nullptr;
static constexpr int WINDOW_WIDTH = 1280;
static constexpr int WINDOW_HEIGHT = 768;
static int app_key_to_glfw(UI::Key button) {
    switch (button) {
                case UI::Key::SPACE: return GLFW_KEY_SPACE;
        case UI::Key::APOSTROPHE: return GLFW_KEY_APOSTROPHE;
        case UI::Key::COMMA: return GLFW_KEY_COMMA;
        case UI::Key::HYPHEN: return GLFW_KEY_MINUS;
        case UI::Key::PERIOD: return GLFW_KEY_PERIOD;
        case UI::Key::SLASH: return GLFW_KEY_SLASH;
        case UI::Key::SEMICOLON: return GLFW_KEY_SEMICOLON;
        case UI::Key::EQUAL: return GLFW_KEY_EQUAL;
        case UI::Key::A: return GLFW_KEY_A;
        case UI::Key::B: return GLFW_KEY_B;
        case UI::Key::C: return GLFW_KEY_C;
        case UI::Key::D: return GLFW_KEY_D;
        case UI::Key::E: return GLFW_KEY_E;
        case UI::Key::F: return GLFW_KEY_F;
        case UI::Key::G: return GLFW_KEY_G;
        case UI::Key::H: return GLFW_KEY_H;
        case UI::Key::I: return GLFW_KEY_I;
        case UI::Key::J: return GLFW_KEY_J;
        case UI::Key::K: return GLFW_KEY_K;
        case UI::Key::L: return GLFW_KEY_L;
        case UI::Key::M: return GLFW_KEY_M;
        case UI::Key::N: return GLFW_KEY_N;
        case UI::Key::O: return GLFW_KEY_O;
        case UI::Key::P: return GLFW_KEY_P;
        case UI::Key::Q: return GLFW_KEY_Q;
        case UI::Key::R: return GLFW_KEY_R;
        case UI::Key::S: return GLFW_KEY_S;
        case UI::Key::T: return GLFW_KEY_T;
        case UI::Key::U: return GLFW_KEY_U;
        case UI::Key::V: return GLFW_KEY_V;
        case UI::Key::W: return GLFW_KEY_W;
        case UI::Key::X: return GLFW_KEY_X;
        case UI::Key::Y: return GLFW_KEY_Y;
        case UI::Key::Z: return GLFW_KEY_Z;
        case UI::Key::LEFT_BRACKET: return GLFW_KEY_LEFT_BRACKET;
        case UI::Key::BACKSLASH: return GLFW_KEY_BACKSLASH;
        case UI::Key::RIGHT_BRACKET: return GLFW_KEY_RIGHT_BRACKET;
        case UI::Key::GRAVE: return GLFW_KEY_GRAVE_ACCENT;
        case UI::Key::ESCAPE: return GLFW_KEY_ESCAPE;
        case UI::Key::ENTER: return GLFW_KEY_ENTER;
        case UI::Key::TAB: return GLFW_KEY_TAB;
        case UI::Key::BACKSPACE: return GLFW_KEY_BACKSPACE;
        case UI::Key::INSERT: return GLFW_KEY_INSERT;
        case UI::Key::DELETE: return GLFW_KEY_DELETE;
        case UI::Key::RIGHT: return GLFW_KEY_RIGHT;
        case UI::Key::LEFT: return GLFW_KEY_LEFT;
        case UI::Key::DOWN: return GLFW_KEY_DOWN;
        case UI::Key::UP: return GLFW_KEY_UP;
        case UI::Key::PAGE_UP: return GLFW_KEY_PAGE_UP;
        case UI::Key::PAGE_DOWN: return GLFW_KEY_PAGE_DOWN;
        case UI::Key::HOME: return GLFW_KEY_HOME;
        case UI::Key::END: return GLFW_KEY_END;
        case UI::Key::PAUSE: return GLFW_KEY_PAUSE;
        case UI::Key::F1: return GLFW_KEY_F1;
        case UI::Key::F2: return GLFW_KEY_F2;
        case UI::Key::F3: return GLFW_KEY_F3;
        case UI::Key::F4: return GLFW_KEY_F4;
        case UI::Key::F5: return GLFW_KEY_F5;
        case UI::Key::F6: return GLFW_KEY_F6;
        case UI::Key::F7: return GLFW_KEY_F7;
        case UI::Key::F8: return GLFW_KEY_F8;
        case UI::Key::F9: return GLFW_KEY_F9;
        case UI::Key::F10: return GLFW_KEY_F10;
        case UI::Key::F11: return GLFW_KEY_F11;
        case UI::Key::F12: return GLFW_KEY_F12;
        case UI::Key::NUM_0: return GLFW_KEY_KP_0;
        case UI::Key::NUM_1: return GLFW_KEY_KP_1;
        case UI::Key::NUM_2: return GLFW_KEY_KP_2;
        case UI::Key::NUM_3: return GLFW_KEY_KP_3;
        case UI::Key::NUM_4: return GLFW_KEY_KP_4;
        case UI::Key::NUM_5: return GLFW_KEY_KP_5;
        case UI::Key::NUM_6: return GLFW_KEY_KP_6;
        case UI::Key::NUM_7: return GLFW_KEY_KP_7;
        case UI::Key::NUM_8: return GLFW_KEY_KP_8;
        case UI::Key::NUM_9: return GLFW_KEY_KP_9;
        case UI::Key::DIVIDE: return GLFW_KEY_KP_DIVIDE;
        case UI::Key::MULTIPLY: return GLFW_KEY_KP_MULTIPLY;
        case UI::Key::SUBTRACT: return GLFW_KEY_KP_SUBTRACT;
        case UI::Key::ADD: return GLFW_KEY_KP_ADD;
        case UI::Key::LEFT_SHIFT: return GLFW_KEY_LEFT_SHIFT;
        case UI::Key::LEFT_CONTROL: return GLFW_KEY_LEFT_CONTROL;
        case UI::Key::LEFT_ALT: return GLFW_KEY_LEFT_ALT;
        case UI::Key::LEFT_SYSTEM: return GLFW_KEY_LEFT_SUPER;
        case UI::Key::RIGHT_SHIFT: return GLFW_KEY_RIGHT_SHIFT;
        case UI::Key::RIGHT_CONTROL: return GLFW_KEY_RIGHT_CONTROL;
        case UI::Key::RIGHT_ALT: return GLFW_KEY_RIGHT_ALT;
        case UI::Key::RIGHT_SYSTEM: return GLFW_KEY_RIGHT_SUPER;
        case UI::Key::MENU: return GLFW_KEY_MENU;
        default: return GLFW_KEY_UNKNOWN;
    }
}

static UI::Key glfw_to_app_key(int button) {
    switch(button) {
        case GLFW_KEY_SPACE: return UI::Key::SPACE;
        case GLFW_KEY_APOSTROPHE: return UI::Key::APOSTROPHE;
        case GLFW_KEY_COMMA: return UI::Key::COMMA;
        case GLFW_KEY_MINUS: return UI::Key::HYPHEN;
        case GLFW_KEY_PERIOD: return UI::Key::PERIOD;
        case GLFW_KEY_SLASH: return UI::Key::SLASH;
        case GLFW_KEY_0: return UI::Key::NUM_0;
        case GLFW_KEY_1: return UI::Key::NUM_1;
        case GLFW_KEY_2: return UI::Key::NUM_2;
        case GLFW_KEY_3: return UI::Key::NUM_3;
        case GLFW_KEY_4: return UI::Key::NUM_4;
        case GLFW_KEY_5: return UI::Key::NUM_5;
        case GLFW_KEY_6: return UI::Key::NUM_6;
        case GLFW_KEY_7: return UI::Key::NUM_7;
        case GLFW_KEY_8: return UI::Key::NUM_8;
        case GLFW_KEY_9: return UI::Key::NUM_9;
        case GLFW_KEY_SEMICOLON: return UI::Key::SEMICOLON;
        case GLFW_KEY_EQUAL: return UI::Key::EQUAL;
        case GLFW_KEY_A: return UI::Key::A;
        case GLFW_KEY_B: return UI::Key::B;
        case GLFW_KEY_C: return UI::Key::C;
        case GLFW_KEY_D: return UI::Key::D;
        case GLFW_KEY_E: return UI::Key::E;
        case GLFW_KEY_F: return UI::Key::F;
        case GLFW_KEY_G: return UI::Key::G;
        case GLFW_KEY_H: return UI::Key::H;
        case GLFW_KEY_I: return UI::Key::I;
        case GLFW_KEY_J: return UI::Key::J;
        case GLFW_KEY_K: return UI::Key::K;
        case GLFW_KEY_L: return UI::Key::L;
        case GLFW_KEY_M: return UI::Key::M;
        case GLFW_KEY_N: return UI::Key::N;
        case GLFW_KEY_O: return UI::Key::O;
        case GLFW_KEY_P: return UI::Key::P;
        case GLFW_KEY_Q: return UI::Key::Q;
        case GLFW_KEY_R: return UI::Key::R;
        case GLFW_KEY_S: return UI::Key::S;
        case GLFW_KEY_T: return UI::Key::T;
        case GLFW_KEY_U: return UI::Key::U;
        case GLFW_KEY_V: return UI::Key::V;
        case GLFW_KEY_W: return UI::Key::W;
        case GLFW_KEY_X: return UI::Key::X;
        case GLFW_KEY_Y: return UI::Key::Y;
        case GLFW_KEY_Z: return UI::Key::Z;
        case GLFW_KEY_LEFT_BRACKET: return UI::Key::LEFT_BRACKET;
        case GLFW_KEY_BACKSLASH: return UI::Key::BACKSLASH;
        case GLFW_KEY_RIGHT_BRACKET: return UI::Key::RIGHT_BRACKET;
        case GLFW_KEY_GRAVE_ACCENT: return UI::Key::GRAVE;
        case GLFW_KEY_WORLD_1: return UI::Key::UNKNOWN;
        case GLFW_KEY_WORLD_2: return UI::Key::UNKNOWN;
        case GLFW_KEY_ESCAPE: return UI::Key::ESCAPE;
        case GLFW_KEY_ENTER: return UI::Key::ENTER;
        case GLFW_KEY_TAB: return UI::Key::TAB;
        case GLFW_KEY_BACKSPACE: return UI::Key::BACKSPACE;
        case GLFW_KEY_INSERT: return UI::Key::INSERT;
        case GLFW_KEY_DELETE: return UI::Key::DELETE;
        case GLFW_KEY_RIGHT: return UI::Key::RIGHT;
        case GLFW_KEY_LEFT: return UI::Key::LEFT;
        case GLFW_KEY_DOWN: return UI::Key::DOWN;
        case GLFW_KEY_UP: return UI::Key::UP;
        case GLFW_KEY_PAGE_UP: return UI::Key::PAGE_UP;
        case GLFW_KEY_PAGE_DOWN: return UI::Key::PAGE_DOWN;
        case GLFW_KEY_HOME: return UI::Key::HOME;
        case GLFW_KEY_END: return UI::Key::END;
        case GLFW_KEY_CAPS_LOCK: return UI::Key::UNKNOWN;
        case GLFW_KEY_SCROLL_LOCK: return UI::Key::UNKNOWN;
        case GLFW_KEY_NUM_LOCK: return UI::Key::UNKNOWN;
        case GLFW_KEY_PRINT_SCREEN: return UI::Key::UNKNOWN;
        case GLFW_KEY_PAUSE: return UI::Key::PAUSE;
        case GLFW_KEY_F1: return UI::Key::F1;
        case GLFW_KEY_F2: return UI::Key::F2;
        case GLFW_KEY_F3: return UI::Key::F3;
        case GLFW_KEY_F4: return UI::Key::F4;
        case GLFW_KEY_F5: return UI::Key::F5;
        case GLFW_KEY_F6: return UI::Key::F6;
        case GLFW_KEY_F7: return UI::Key::F7;
        case GLFW_KEY_F8: return UI::Key::F8;
        case GLFW_KEY_F9: return UI::Key::F9;
        case GLFW_KEY_F10: return UI::Key::F10;
        case GLFW_KEY_F11: return UI::Key::F11;
        case GLFW_KEY_F12: return UI::Key::F12;
        case GLFW_KEY_F13: return UI::Key::UNKNOWN;
        case GLFW_KEY_F14: return UI::Key::UNKNOWN;
        case GLFW_KEY_F15: return UI::Key::UNKNOWN;
        case GLFW_KEY_F16: return UI::Key::UNKNOWN;
        case GLFW_KEY_F17: return UI::Key::UNKNOWN;
        case GLFW_KEY_F18: return UI::Key::UNKNOWN;
        case GLFW_KEY_F19: return UI::Key::UNKNOWN;
        case GLFW_KEY_F20: return UI::Key::UNKNOWN;
        case GLFW_KEY_F21: return UI::Key::UNKNOWN;
        case GLFW_KEY_F22: return UI::Key::UNKNOWN;
        case GLFW_KEY_F23: return UI::Key::UNKNOWN;
        case GLFW_KEY_F24: return UI::Key::UNKNOWN;
        case GLFW_KEY_F25: return UI::Key::UNKNOWN;
        case GLFW_KEY_KP_0: return UI::Key::NUM_0;
        case GLFW_KEY_KP_1: return UI::Key::NUM_1;
        case GLFW_KEY_KP_2: return UI::Key::NUM_2;
        case GLFW_KEY_KP_3: return UI::Key::NUM_3;
        case GLFW_KEY_KP_4: return UI::Key::NUM_4;
        case GLFW_KEY_KP_5: return UI::Key::NUM_5;
        case GLFW_KEY_KP_6: return UI::Key::NUM_6;
        case GLFW_KEY_KP_7: return UI::Key::NUM_7;
        case GLFW_KEY_KP_8: return UI::Key::NUM_8;
        case GLFW_KEY_KP_9: return UI::Key::NUM_9;
        case GLFW_KEY_KP_DECIMAL: return UI::Key::UNKNOWN;
        case GLFW_KEY_KP_DIVIDE: return UI::Key::DIVIDE;
        case GLFW_KEY_KP_MULTIPLY: return UI::Key::MULTIPLY;
        case GLFW_KEY_KP_SUBTRACT: return UI::Key::SUBTRACT;
        case GLFW_KEY_KP_ADD: return UI::Key::ADD;
        case GLFW_KEY_KP_ENTER: return UI::Key::ENTER;
        case GLFW_KEY_KP_EQUAL: return UI::Key::EQUAL;
        case GLFW_KEY_LEFT_SHIFT: return UI::Key::LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL: return UI::Key::LEFT_CONTROL;
        case GLFW_KEY_LEFT_ALT: return UI::Key::LEFT_ALT;
        case GLFW_KEY_LEFT_SUPER: return UI::Key::LEFT_SYSTEM;
        case GLFW_KEY_RIGHT_SHIFT: return UI::Key::RIGHT_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL: return UI::Key::RIGHT_CONTROL;
        case GLFW_KEY_RIGHT_ALT: return UI::Key::RIGHT_ALT;
        case GLFW_KEY_RIGHT_SUPER: return UI::Key::RIGHT_SYSTEM;
        case GLFW_KEY_MENU: return UI::Key::MENU;
        default: return UI::Key::UNKNOWN;
    }
}

bool key_held_fn(UI::Key key)
{
    if (!g_window)
        return false;
    return glfwGetKey(g_window, app_key_to_glfw(key));
    //return sf::Keyboard::isKeyPressed((sf::Keyboard::Key) key);
}

Application::Application()
{
    m_ui.add_view(std::make_shared<WelcomeView>(m_data, key_held_fn));
}

Application::~Application() {

}

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error(" << error << "): " << description << std::endl;
}


static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *manager = static_cast<UI::Manager*>(glfwGetWindowUserPointer(window));
    auto &io = ImGui::GetIO();
    if (io.WantCaptureKeyboard)
        return;

    auto app_key = glfw_to_app_key(key);
    if (app_key == UI::Key::UNKNOWN)
        return;

    std::cout << "Hee hoo" << std::endl;
    if (action == GLFW_PRESS)
    {
        manager->do_key_press(app_key, {
            (mods & GLFW_MOD_CONTROL) != 0,
            (mods & GLFW_MOD_SHIFT) != 0,
            (mods & GLFW_MOD_ALT) != 0
        });
    } else if (action == GLFW_RELEASE)
    {
        manager->do_key_release(app_key, {
            (mods & GLFW_MOD_CONTROL) != 0,
            (mods & GLFW_MOD_SHIFT) != 0,
            (mods & GLFW_MOD_ALT) != 0
        });
    }
}

static void glfw_char_callback(GLFWwindow *window, unsigned int codepoint) {
    auto *manager = static_cast<UI::Manager*>(glfwGetWindowUserPointer(window));
    auto &io = ImGui::GetIO();
    if (io.WantCaptureKeyboard)
        return;

    manager->do_char_write(codepoint);
}
static void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    auto *manager = static_cast<UI::Manager*>(glfwGetWindowUserPointer(window));
    auto &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    if (action == GLFW_PRESS) {
        manager->do_mouse_press(static_cast<UI::MouseButton>(button), mx, my);
    } else if (action == GLFW_RELEASE) {
        manager->do_mouse_release(static_cast<UI::MouseButton>(button), mx, my);
    }
}

static void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    auto *manager = static_cast<UI::Manager*>(glfwGetWindowUserPointer(window));
    auto &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;

    manager->do_mouse_moved(xpos, ypos);
}

static void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto *manager = static_cast<UI::Manager*>(glfwGetWindowUserPointer(window));
    auto &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;

    if (xoffset != 0)
        manager->do_mouse_wheel(true, xoffset);
    if (yoffset != 0)
        manager->do_mouse_wheel(false, xoffset);
}

static void glfw_window_size_callback(GLFWwindow *window, int width, int height) {
    auto *manager = static_cast<UI::Manager*>(glfwGetWindowUserPointer(window));
    manager->do_resize(width, height);
}

int Application::run(int argc, char **argv) {
    if (!pfd::settings::available()) {
        std::cerr << "Can't even get portable file dialogs in this economy smh my head" << std::endl;
        return 1;
    }

    // Initialize GLFW.
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window.
    g_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Traveler's Toolkit", nullptr, nullptr);
    if (!g_window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwSetKeyCallback(g_window, glfw_key_callback);
    glfwSetCharCallback(g_window, glfw_char_callback);
    glfwSetMouseButtonCallback(g_window, glfw_mouse_button_callback);
    glfwSetScrollCallback(g_window, glfw_scroll_callback);
    glfwSetCursorPosCallback(g_window, glfw_cursor_position_callback);
    glfwSetWindowSizeCallback(g_window, glfw_window_size_callback);

    glfwSetWindowUserPointer(g_window, &m_ui);

    // Initialize OpenGL.
    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(1);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    // Initialize ImGUI.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsDark();

    // Copied over from imgui's glfw/opengl3 example, no clue what it does.
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Finalize ImGUI initialization.
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init();

    // Initialize project.
    std::string twoc_root;
    if (argc <= 1) {
        twoc_root = pfd::select_folder("Select extracted game root...").result();
        if (twoc_root.empty())
            return 1;
    } else {
        twoc_root.append(argv[1]);
        if (!std::filesystem::is_directory(twoc_root))
            return 1;
        if (!std::filesystem::exists(twoc_root))
            return 1;
    }

    if (!m_data.Project.load(twoc_root))
        return 1;


    m_ui.do_resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_ui.change_view(0);

    double delta_time = 0.f;
    double current_time = glfwGetTime();
    double last_time = current_time;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    while (!glfwWindowShouldClose(g_window))
    {
        glfwPollEvents();

        // process pending actions
        while (!m_action_queue.empty()) {
            auto action = m_action_queue.front();
            m_ui.do_process_action(action);
            m_action_queue.pop();
        }

        // update imgui state
        current_time = glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;

        int display_w, display_h;
        glfwGetFramebufferSize(g_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        // update and render UI
        m_ui.do_update(delta_time);
        m_ui.do_render();
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // Flip.
        glfwSwapBuffers(g_window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    glfwDestroyWindow(g_window);
    glfwTerminate();
    return 0;
}

}