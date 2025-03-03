extern "C" {
#include <glad/glad.h>
}
#include <SFML/OpenGL.hpp>
#include <cstring>
#include <iostream>
#include <imgui.h>
#include <cmath>
#include <portable-file-dialogs.h>
#include <filesystem>
#include <regex>
#include <algorithm>
#include <cctype>

#include "file_binary_reader.h"
#include "twoc/executable.h"
#include "imgui-SFML.h"
#include "imgui_impl_opengl3.h"

#include "views/welcome/welcome_view.h"
#include "application.h"

static constexpr int WINDOW_WIDTH = 1280;
static constexpr int WINDOW_HEIGHT = 768; 
Application::Application() 
{
    m_ui.add_view(std::make_shared<WelcomeView>(m_data));
}

Application::~Application() {

}

int Application::run(int argc, char **argv) {

    if (!pfd::settings::available())
    {
        std::cerr << "Can't even get portable file dialogs in this economy smh my head" << std::endl;
        return 1;
    }

    std::string twoc_root = pfd::select_folder("Select extracted game root...").result();
    if (twoc_root.empty())
        return 1;

    if (!m_data.Project.load(twoc_root))
        return 1;

    // initialize window and opengl
    sf::ContextSettings settings(16, 8, 4, 4, 3, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug);
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Traveler's Toolkit", sf::Style::Default, settings);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction));
    window.setFramerateLimit(0);
    
    // initialize imgui
    if (!ImGui::SFML::Init(window))
        return 1;
    ImGui_ImplOpenGL3_Init();
//    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    m_ui.do_resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_ui.change_view(0);
    float dt = 0.f;

    sf::Clock delta_clock;   
    while (window.isOpen())
    {
        // poll events
        ImGuiIO &io = ImGui::GetIO();
        for (sf::Event event; window.pollEvent(event);)
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
                goto exited;
            } else if (event.type == sf::Event::Resized)
            {
                m_ui.do_resize(event.size.width, event.size.height);
            } else if (event.type == sf::Event::MouseButtonPressed || 
                    event.type == sf::Event::MouseButtonReleased || 
                    event.type == sf::Event::MouseEntered || 
                    event.type == sf::Event::MouseLeft || 
                    event.type == sf::Event::MouseMoved)
            {
                if (!io.WantCaptureMouse)
                {
                    if (event.type == sf::Event::MouseButtonPressed)
                    {
                        m_ui.do_mouse_press(static_cast<UI::MouseButton>(event.mouseButton.button), event.mouseButton.x, event.mouseButton.y);
                    } else if (event.type == sf::Event::MouseButtonReleased)
                    {
                        m_ui.do_mouse_release(static_cast<UI::MouseButton>(event.mouseButton.button), event.mouseButton.x, event.mouseButton.y);
                    } else if (event.type == sf::Event::MouseMoved)
                    {
                        m_ui.do_mouse_moved(event.mouseMove.x, event.mouseMove.y);
                    }
                }
            } else if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (!io.WantCaptureMouse)
                {
                    m_ui.do_mouse_moved(event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel, event.mouseWheelScroll.delta);
                }
            } else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
            {
                if (!io.WantCaptureKeyboard)
                {
                    if (event.type == sf::Event::KeyPressed)
                    {
                        m_ui.do_key_press(static_cast<UI::Key>(event.key.code), { event.key.control, event.key.shift, event.key.alt });
                    } else if (event.type == sf::Event::KeyReleased)
                    {
                        m_ui.do_key_release(static_cast<UI::Key>(event.key.code), { event.key.control, event.key.shift, event.key.alt });
                    }
                }                
            } else if (event.type == sf::Event::TextEntered)
            {
                if (!io.WantCaptureKeyboard)
                {
                    m_ui.do_char_write(event.text.unicode);
                }
            }
        }

        // update imgui state
        sf::Time elapsed = delta_clock.restart();
        dt = elapsed.asSeconds();

        ImGui::SFML::Update(window, elapsed);
        ImGui_ImplOpenGL3_NewFrame();

        // update and render UI
        m_ui.do_update(dt);
        m_ui.do_render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // flip
        window.display();
    }
exited:
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::SFML::Shutdown();
    return 0;
}
