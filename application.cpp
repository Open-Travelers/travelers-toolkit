extern "C" {
#include <glad/glad.h>
}
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <imgui.h>
#include <cmath>

#include "application.h"
#include "binary_stream.h"
#include "imgui-SFML.h"
#include "imgui_impl_opengl3.h"
#include "nu/scene.h"
#include "math/mat4x4.h"
#include "math/vec3.h"
#include "scene_renderer.h"

#include "portable-file-dialogs.h"
Application::Application() {

}

Application::~Application() {

}
static std::string comeonman ="No textures loaded";
int Application::run(int argc, char **argv) {

    if (!pfd::settings::available())
    {
        std::cerr << "Can't even get portable file dialogs in this economy smh my head" << std::endl;
        return 1;
    }

    // initialize window and opengl
    sf::ContextSettings settings(16, 8, 4, 4, 3, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug);

    sf::RenderWindow window(sf::VideoMode(1280, 768), "Traveler's Toolkit", sf::Style::Default, settings);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction));
    window.setFramerateLimit(144);
    
    Nu::Scene scene;
    NuSceneRenderer scene_renderer;

    // initialize imgui
    if (!ImGui::SFML::Init(window))
        return 1;
    ImGui_ImplOpenGL3_Init();

    
    bool loaded = false;
    sf::Clock deltaClock;
    float rot = 0;

    int last_mouse_x = 0;
    int last_mouse_y = 0;
    int mouse_x = 0;
    int mouse_y = 0;
    Vec3 cam_pos(0, 0, 40);
    float cam_rot = 0;
    float move_speed = 0.5f;
    float rot_speed = 0.1f;
    int txv_current_item = 0;
    std::vector<std::string> txv_items;
    while (window.isOpen())
    {
        // poll events
        ImGuiIO &io = ImGui::GetIO();
        for (sf::Event event; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                goto exited;
            } else if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, window.getSize().x, window.getSize().y);
            } else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::MouseEntered || event.type == sf::Event::MouseLeft || event.type == sf::Event::MouseMoved)
            {
                ImGui::SFML::ProcessEvent(window, event);
                if (!io.WantCaptureMouse)
                {

                    if (event.type == sf::Event::MouseButtonPressed)
                    {
                        if (event.mouseButton.button == sf::Mouse::Button::Left)
                        {

                        } else if (event.mouseButton.button == sf::Mouse::Button::Middle)
                        {

                        } else if (event.mouseButton.button == sf::Mouse::Button::Right)
                        {

                        }
                    } else if (event.type == sf::Event::MouseButtonReleased)
                    {
                        if (event.mouseButton.button == sf::Mouse::Button::Left)
                        {

                        } else if (event.mouseButton.button == sf::Mouse::Button::Middle)
                        {

                        } else if (event.mouseButton.button == sf::Mouse::Button::Right)
                        {

                        }
                    } else if (event.type == sf::Event::MouseMoved)
                    {

                        last_mouse_x = mouse_x;
                        last_mouse_y = mouse_y;
                        mouse_x = event.mouseMove.x;
                        mouse_y = event.mouseMove.y;
                    }
                }
            } else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
            {
                if (!io.WantCaptureKeyboard)
                {
                    if (event.type == sf::Event::KeyPressed)
                    {

                    } else if (event.type == sf::Event::KeyReleased)
                    {

                    }
                }                
            }
        }

        // update imgui state
        sf::Time elapsed = deltaClock.restart();

        if (!io.WantCaptureKeyboard)
        {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                cam_rot -= rot_speed / elapsed.asMilliseconds();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                cam_rot += rot_speed / elapsed.asMilliseconds();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                cam_pos.set_x(cam_pos.x() + cos(cam_rot) * move_speed);
                cam_pos.set_z(cam_pos.z() + sin(cam_rot) * move_speed);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                cam_pos.set_x(cam_pos.x() - cos(cam_rot) * move_speed);
                cam_pos.set_z(cam_pos.z() - sin(cam_rot) * move_speed);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                cam_pos.set_y(cam_pos.y() + move_speed / elapsed.asMilliseconds());
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                cam_pos.set_y(cam_pos.y() - move_speed / elapsed.asMilliseconds());
        }

        ImGui::SFML::Update(window, elapsed);
        ImGui_ImplOpenGL3_NewFrame();

        // build UI
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Load Scenery"))
                {
                    auto file_dialog_result = pfd::open_file("Choose GSC/NUS file to read", pfd::path::home(), {
                        "GSC Files (.gsc)", "*.gsc *.GSC",
                        "NUS Files (.nus)", "*.nus *.NUS",
                        "All files", "*.*"
                    });

                    auto files = file_dialog_result.result();
                    if (files.size() == 1)
                    {
                        auto path = files[0];
                        BinaryStream stream;
                        if (!stream.open(path))
                        {
                            std::cerr << "Failed to open file at path '" << path << "'" << std::endl;
                            goto exit;
                        }

                        // try to read the scene
                        if (!scene.read(stream))
                        {
                            std::cerr << "Failed to parse NuScene at path '" << path << "'" << std::endl;
                            goto exit;
                        }
                        std::cout << "NuScene parsed successfully" << std::endl;
                        
                        if (!scene_renderer.load(scene))
                        {
                            std::cerr << "Failed to load NuScene for rendering" << std::endl;
                            goto exit;
                        }
                        std::cout << "NuScene loaded for rendering successfully" << std::endl;
                        txv_items.clear();
                        for (int i = 0; i < scene.get_textures().size(); i++)
                            txv_items.push_back("Texture " + std::to_string(i));
                        txv_current_item = 0;
                    }
                }
exit:
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (ImGui::Begin("Misc. Settings"))
        {
            ImGui::SliderFloat("Move speed", &move_speed, 0.1, 20.f);
            ImGui::SliderFloat("Rot speed", &rot_speed, 0.1, 5.f);
        }
        ImGui::End();

        if (ImGui::Begin("Texture Viewer"))
        {
            if (txv_items.size() == 0)
            {
                ImGui::Text("No textures loaded");
            } else {
                if (ImGui::Button("Prev") && txv_current_item > 0) txv_current_item--;
                ImGui::SameLine();
                if (ImGui::Button("Next") && txv_current_item < txv_items.size() - 1) txv_current_item++;

                ImGui::ListBox("##Texture ID", &txv_current_item, [](void* user_data, int which) -> const char* { 
                    return (((std::string*) user_data)[which]).c_str();
                }, (void*) txv_items.data(), txv_items.size());
                auto texture_raw = scene_renderer.get_texture(txv_current_item);
                if (texture_raw.has_value())
                {
                    ImGui::SameLine();
                    ImGui::BeginChild("texture-view");
                    
                    Nu::Texture tex = scene.get_textures()[txv_current_item];
                    ImVec2 wsize((float) tex.width(), (float) tex.height());

                    ImGui::Image((ImTextureID) texture_raw.value(), wsize, ImVec2(0, 0), ImVec2(1, 1));
                    ImGui::EndChild();
                }
            }
        }
        ImGui::End();
        // render 3d
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        
        glClearColor(0.2f, 0.7f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Vec3 cam_to(cam_pos.x() + cos(cam_rot) * 50, cam_pos.y(), cam_pos.z() + sin(cam_rot) * 50);
        Mat4x4 view = Mat4x4::LookAt(cam_pos, cam_to, Vec3(0, 1, 0));
        Mat4x4 projection = Mat4x4::PerspectiveProjection(window.getSize().x / (float) window.getSize().y, 60 * 0.01745329252, 0.01f, 1000.f);

        rot += elapsed.asSeconds();
        
        scene_renderer.render_scene_all(scene, view, projection);
        
        // render UI

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