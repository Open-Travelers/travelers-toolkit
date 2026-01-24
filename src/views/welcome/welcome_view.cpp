#include <cstdint>
#include <imgui.h>
#include <portable-file-dialogs.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <filesystem>
#include <vector>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "file_binary_reader.h"
#include "twoc/nu/scene.h"
#include "twoc/dat/file.h"

#include "welcome_view.h"

WelcomeView::WelcomeView(App::Data &app, std::function<bool(UI::Key)> key_held_fn) : UI::View(app, key_held_fn),
    m_camera(glm::vec3 { 0, 0, 0 }, glm::vec3 { 0, 0, 5 }), m_projection_matrix(glm::perspectiveFov(glm::radians<float>(90.f), 1280.f, 768.f, 0.01f, 100.f))
{

}

WelcomeView::~WelcomeView()
{
    
}

bool WelcomeView::load_scenes()
{
    if (m_crate_scene)
        delete m_crate_scene;
    m_crate_scene = nullptr;

    if (m_wumpa_scene)
        delete m_wumpa_scene;
    m_wumpa_scene = nullptr;

    std::unique_ptr<Twoc::BinaryReader> crate_reader = m_app.Project.find_file("stuff/crates.nus");
    if (crate_reader)
    {
        m_crate_scene = Twoc::Nu::Scene::from_reader(*crate_reader);
        if (!m_crate_scene)
        {
            std::cerr << "Could not read crate file!" << std::endl;
            return false;
        }
    } else {
        std::cerr << "Could not find crate file!" << std::endl;
        return false;
    }

    std::unique_ptr<Twoc::BinaryReader> wumpa_reader = m_app.Project.find_file("stuff/wumpa.nus");
    if (wumpa_reader)
    {
        m_wumpa_scene = Twoc::Nu::Scene::from_reader(*wumpa_reader);
        if (!m_wumpa_scene)
        {
            std::cerr << "Could not read wumpa file!" << std::endl;
            delete m_crate_scene;
            m_crate_scene = nullptr;
            return false;
        }
    } else {
        std::cerr << "Could not find wumpa file!" << std::endl;
        delete m_crate_scene;
        m_crate_scene = nullptr;
        return false;
    }
    return true;
}
void WelcomeView::on_load(int width, int height)
{
    load_scenes();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    m_camera.set_position({4, 0, 0});
    m_camera.set_target({0, 0, 0});
}

UI::ViewChange WelcomeView::on_unload()
{
    return UI::ViewChange::CHANGE;
}

void WelcomeView::on_mouse_press(UI::MouseButton button, double x, double y)
{
    if (button == UI::MouseButton::RIGHT)
        m_rotating_camera = true;
}

void WelcomeView::on_mouse_release(UI::MouseButton button, double x, double y)
{
    if (button == UI::MouseButton::RIGHT)
        m_rotating_camera = false;
}

void WelcomeView::on_mouse_moved(double x, double y)
{
    int dx = x - m_last_mouse_x;
    int dy = y - m_last_mouse_y;
    m_mouse_dx = dx;
    m_mouse_dy = dy;
    m_last_mouse_x = x;
    m_last_mouse_y = y;
}

void WelcomeView::on_update(float dt)
{
    if (m_rotating_camera)
    {
        glm::vec3 direction { 0 };
        if (key_held(UI::Key::W))
            direction -= m_camera.front();
        if (key_held(UI::Key::S))
            direction += m_camera.front();
        if (key_held(UI::Key::A))
            direction -= m_camera.right();
        if (key_held(UI::Key::D))
            direction += m_camera.right();

        if (glm::length(direction) != 0.f)
            direction = glm::normalize(direction);
        m_camera.move(direction * 20.f * dt);

        m_camera.add_yaw(m_mouse_dx * dt);
        m_camera.add_pitch(-m_mouse_dy * dt);
        m_camera.set_target_from_angles();

        m_mouse_dx = 0;
        m_mouse_dy = 0;
    }
}

void WelcomeView::on_resize(int width, int height)
{
    m_projection_matrix = glm::perspectiveFov(glm::radians(90.f), (float)width, (float)height, 0.01f, 5000.f);
    glViewport(0, 0, width, height);
}

bool WelcomeView::load_scene(Twoc::Nu::Scene *scene)
{
    if (scene == nullptr)
    {
        (void) pfd::message("Error", "Could not load scene!", pfd::choice::ok, pfd::icon::error);
    } else {
        if (!m_renderer.load(scene))
        {
            (void) pfd::message("Error", "Could not load scene into renderer!", pfd::choice::ok, pfd::icon::error);
            return false;
        } else {
            if (m_scene != nullptr)
                delete m_scene;
            m_scene = scene;

            m_texture_names.clear();
            for (size_t i = 0; i < m_scene->texture_count(); i++)
            {
                auto const& tex = m_scene->texture(i);
                std::stringstream str;
                str << "Texture " << i << ": " << tex.max_width() << "x" << tex.max_height() << " #" << tex.type();
                m_texture_names.push_back(str.str());
            }
            m_texture_selected = -1;
        }
    }
    return true;
}

void WelcomeView::on_render()
{
    static bool in_custom_level_window = false;
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    if (m_scene)
        m_renderer.render(m_camera.view_matrix(), m_projection_matrix);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load Game Root"))
            {
                std::string twoc_root = pfd::select_folder("Select extracted game root...").result();
                if (twoc_root.empty())
                {
                    (void) pfd::message("Error", "Can't select empty folder!", pfd::choice::ok, pfd::icon::error);
                } else {
                    if (!m_app.Project.load(twoc_root))
                    {
                        (void) pfd::message("Error", "Couldn't load game root!", pfd::choice::ok, pfd::icon::error);
                    } else {
                        load_scenes();
                    }
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Level"))
        {
            if (ImGui::MenuItem("Custom Level Selector"))
            {
                if (m_app.Project.executable() != nullptr)
                    in_custom_level_window = true;
                else
                    (void) pfd::message("Error", "Need to load root first!", pfd::choice::ok, pfd::icon::error);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (m_app.Project.executable() == nullptr)
        return;

    if (in_custom_level_window)
    {
        static std::string scene_path;
        if (ImGui::Begin("Select Individual Level Files"))
        {
            ImGui::Text("%s", scene_path.c_str());
            ImGui::SameLine();
            if (ImGui::Button("..."))
            {
                const std::vector<std::string> scene_filter = {
                    "Nu Scene", "*.nus|*.NUS",
                    "All Files", "*"
                };

                auto result = pfd::open_file("Select Scene", scene_path, scene_filter).result();
                if (!result.empty())
                {
                    scene_path = result[0];
                }
            }
            if (ImGui::Button("Load"))
            {
                if (!scene_path.empty())
                {
                    FileBinaryReader scene_reader(m_app.Project.endianness());
                    if (!scene_reader.open(scene_path))
                    {
                        (void) pfd::message("Error", "Failed to open file '" + scene_path + "'!", pfd::choice::ok, pfd::icon::error);
                    } else {
                        Twoc::Nu::Scene *scene = Twoc::Nu::Scene::from_reader(scene_reader);
                        load_scene(scene);
                    }
                }
                in_custom_level_window = false;
            }

        }
        ImGui::End();
    }

    if (ImGui::Begin("Levels"))
    {
        if (ImGui::TreeNodeEx("Root", ImGuiTreeNodeFlags_SpanFullWidth))
        {
            int i = 0;
            for (auto& lvl : m_app.Project.executable()->level_data())
            {
                ImGui::PushID(i);

                bool exists = m_app.Project.does_level_exist(i);
                if (!exists)
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                else
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));

                if (ImGui::TreeNode(lvl.Names[0].c_str()))
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
                    if (exists)
                    {
                        if (ImGui::Button("Load Level"))
                        {
                            std::filesystem::path level_path("levels\\" + lvl.Filepath);

                            std::filesystem::path scene_path = level_path;
                            scene_path += ".nus";

                            std::unique_ptr<Twoc::BinaryReader> scene_reader = m_app.Project.find_file(scene_path);
                            if (!scene_reader)
                            {
                                std::cerr << "Path '" << scene_path << "' doesn't exist!" << std::endl;
                            } else {
                                Twoc::Nu::Scene *scene = Twoc::Nu::Scene::from_reader(*scene_reader);
                                load_scene(scene);
                            }
                        }
                    }

                    if (ImGui::TreeNode("Names"))
                    {
                        for (auto &name : lvl.Names)
                        {
                            ImGui::Text("%s", name.c_str());
                        }
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Model List"))
                    {
                        for (auto &model : lvl.ModelList)
                        {
                            auto character = m_app.Project.executable()->character_data(model);
                            ImGui::Text("%s", character.Name.c_str());
                        }
                        ImGui::TreePop();
                    }

                    ImGui::Text("Prefix: ");
                    ImGui::SameLine();
                    ImGui::Text("%s", lvl.Filepath.c_str());

                    ImGui::InputScalar("Flags", ImGuiDataType_U16, (void*) &lvl.Flags);

                    ImGui::InputScalar("Hub", ImGuiDataType_S8, (void*) &lvl.Hub);
                    ImGui::InputScalar("Character", ImGuiDataType_S16, (void*) &lvl.Character);
                    ImGui::InputScalar("Vehicle", ImGuiDataType_S16, (void*)  &lvl.Vehicle);
                    ImGui::InputScalarN("Time", ImGuiDataType_U32, (void*) &lvl.Time, 3);

                    ImGui::InputFloat3("Start Position",  (float*) &lvl.Start[0]);
                    ImGui::InputFloat3("Bonus Position", (float*) &lvl.Bonus[0]);

                    ImGui::InputScalarN("Music", ImGuiDataType_S16, (void*) &lvl.Music, 2);

                    ImGui::InputFloat("Fog Near", (float*) &lvl.FogNear);
                    ImGui::InputFloat("Fog Far", (float*) &lvl.FogFar);
                    ImGui::InputScalarN("Fog Color", ImGuiDataType_U8, (void*) &lvl.FogColor, 4);
                    ImGui::InputScalarN("Haze Color", ImGuiDataType_U8, (void*) &lvl.HazeColor, 4);
                    ImGui::InputScalar("Total Size", ImGuiDataType_S32, (void*) &lvl.TotalSize);

                    ImGui::InputScalar("Far Clipping Plane", ImGuiDataType_U16, (void*) &lvl.FarClippingPlane);
                    if (lvl.Type == Twoc::GameType::E3)
                    {
                        ImGui::InputFloat3("Clock Position", (float*) &lvl.ClockPosition[0]);
                        ImGui::InputFloat3("Gem Position", (float*) &lvl.GemPosition[0]);
                        ImGui::InputFloat3("Crystal Position", (float*) &lvl.CrystalPosition[0]);

                        ImGui::InputFloat3("Directional Light 1", (float*) &lvl.DirLight1[0]);
                        ImGui::InputFloat3("Directional Light 2", (float*) &lvl.DirLight2[0]);

                        ImGui::InputScalarN("Gradient Start", ImGuiDataType_U8, (void*) &lvl.GradientStart, 4);
                        ImGui::InputScalarN("Gradient End", ImGuiDataType_U8, (void*) &lvl.GradientEnd, 4);

                        ImGui::InputScalar("Unk1", ImGuiDataType_U32, (void*) &lvl.Unk1);
                        ImGui::InputFloat3("Unknown 2", (float*) &lvl.Unk2[0]);
                        ImGui::InputFloat3("Unknown 2", (float*) &lvl.Unk3[0]);
                    }

                    ImGui::TreePop();
                    ImGui::PopStyleColor();
                }

                ImGui::PopStyleColor();
                i++;
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();

    if (ImGui::Begin("Characters"))
    {
        if (ImGui::Button("Load chars.dat")) {
            std::filesystem::path chars_path = m_app.Project.root_path() / "chars.dat";
            FileBinaryReader reader(m_app.Project.endianness());
            if (reader.open(chars_path))
            {
                Twoc::Dat::File f;
                if (!f.read(reader))
                    std::cerr << "Failure reading dat file" << std::endl;
            }
        }
        if (ImGui::TreeNodeEx("Root##2", ImGuiTreeNodeFlags_SpanFullWidth))
        {
            int i = 0;
            for (auto &character : m_app.Project.executable()->character_data())
            {
                ImGui::PushID(i);
                if (ImGui::TreeNode(character.Name.c_str()))
                {
                    ImGui::Text("Path: %s", character.Path.c_str());
                    ImGui::Text("File: %s", character.File.c_str());
                    ImGui::Text("Name: %s", character.Name.c_str());
                    ImGui::Text("Radius: %f", character.Radius);
                    if (character.Type == Twoc::GameType::Retail)
                    {
                        ImGui::InputScalarN("Min", ImGuiDataType_Float, (void*) &character.Min[0], 3);
                        ImGui::InputScalarN("Max", ImGuiDataType_Float, (void*) &character.Max[0], 3);
                        ImGui::InputFloat("Scale", (float*) &character.Scale);
                        ImGui::InputFloat("Shadow Scale", (float*) &character.ShadowScale);
                    } else if (character.Type == Twoc::GameType::E3)
                    {
                        ImGui::InputFloat("Unknown 1", (float*) &character.Unk1);
                        ImGui::InputFloat("Unknown 2", (float*) &character.Unk2);
                        ImGui::InputFloat("Unknown 3", (float*) &character.Unk3);
                        ImGui::InputFloat("Unknown 4", (float*) &character.Unk4);
                    }
                    ImGui::TreePop();
                }
                i++;
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();

    if (m_scene != nullptr)
    {
        if (ImGui::Begin("Scene"))
        {
            if (ImGui::TreeNodeEx("Geometry Objects"))
            {
                int i = 0;
                for (auto const& object : m_scene->geometry_objects())
                {
                    ImGui::PushID(i);
                    glm::vec3 origin = object.origin();
                    auto origin_data = glm::value_ptr(origin);
                    auto const& meshes = object.meshes();

                    if (ImGui::TreeNode("Geometry Object"))
                    {
                        ImGui::InputFloat3("Origin", origin_data);
                        int j = 0;
                        for (auto const& mesh : meshes)
                        {
                            uint32_t material = mesh.material();
                            auto const& primitives = mesh.primitives();
                            auto const& vertices = mesh.vertices();

                            ImGui::PushID(j);
                            if (ImGui::TreeNode("Mesh"))
                            {
                                ImGui::InputScalar("Material", ImGuiDataType_U32, &material);
                                ImGui::TreePop();
                            }
                            ImGui::PopID();
                            j++;
                        }
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                    i++;
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNodeEx("Splines"))
            {
                int i = 0;
                for (auto const& spline : m_scene->splines())
                {
                    std::cout << spline.name() << std::endl;
                    ImGui::PushID(i);
                    if (ImGui::TreeNode(spline.name().c_str()))
                    {
                        int j = 0;
                        for (auto const& pt : spline.points())
                        {
                            glm::vec3 copy = pt;
                            ImGui::PushID(j);
                            ImGui::InputFloat3("Pos", &copy.x);
                            j++;
                            ImGui::PopID();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                    i++;
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Instances"))
            {
                int i = 0;
                for (auto const& instance : m_scene->instances())
                {
                    ImGui::PushID(i);

                    if (ImGui::TreeNode("Instance"))
                    {
                        uint32_t object_index = instance.object_index();
                        uint16_t room_group = instance.room_group();
                        Twoc::Nu::InstanceFlags flags = instance.flags();
                        uint8_t special = instance.special_flag();
                        glm::mat4 transform_matrix = instance.transform_matrix();

                        ImGui::InputScalar("Geometry Object", ImGuiDataType_U32, &object_index);
                        ImGui::InputScalar("Room Group", ImGuiDataType_U16, &room_group);
                        ImGui::InputScalar("Flags", ImGuiDataType_U32, &flags);
                        ImGui::InputScalar("Special Flag", ImGuiDataType_U8, &special);
                        for (int row = 0; row < 4; row++)
                        {
                            glm::vec4 row_vector = glm::row(transform_matrix, row);
                            auto row_data = glm::value_ptr(row_vector);
                            ImGui::InputFloat4("##Matrix", row_data);
                        }

                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                    i++;
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNodeEx("Materials"))
            {
                int i = 0;
                for (auto const& material : m_scene->materials())
                {
                    ImGui::PushID(i);
                    if (ImGui::TreeNode("Material")) 
                    {
                        uint32_t flags = material.flags();
                        glm::vec3 ambient = { material.ambient(0), material.ambient(1), material.ambient(2) };
                        glm::vec3 diffuse = { material.diffuse(0), material.diffuse(1), material.diffuse(2) };
                        uint32_t fx_params_0 = material.fx_params(0);
                        uint32_t fx_params_1 = material.fx_params(1);
                        uint32_t fx_params_2 = material.fx_params(2);
                        uint32_t fx_params_3 = material.fx_params(3);
                        float power = material.power();
                        float alpha = material.alpha();
                        uint32_t texture_id = material.texture_id();
                        int16_t alpha_sort = material.alpha_sort();
                        uint8_t fx_id = material.fx_id();
                        int16_t K = material.K();
                        uint8_t L = material.L();
                        uint8_t animation_mode = material.animation_mode();
                        float dudv[2] = { material.du(), material.dv() };
                        float susv[2] = { material.su(), material.sv() };
                        ImGui::InputScalar("Texture ID", ImGuiDataType_S32, &texture_id);
                        ImGui::InputScalar("Flags", ImGuiDataType_U32, &flags);
                        ImGui::InputFloat3("Ambient", &ambient.x);
                        ImGui::InputFloat3( "Diffuse", &diffuse.x);
                        ImGui::InputScalar("Power", ImGuiDataType_Float, &power);
                        ImGui::InputScalar("Alpha", ImGuiDataType_Float, &alpha);
                        
                        ImGui::InputScalar("Fx. Params 0", ImGuiDataType_U32, &fx_params_0);
                        ImGui::InputScalar("Fx. Params 1", ImGuiDataType_U32, &fx_params_1);
                        ImGui::InputScalar("Fx. Params 2", ImGuiDataType_U32, &fx_params_2);
                        ImGui::InputScalar("Fx. Params 3", ImGuiDataType_U32, &fx_params_3);
                        ImGui::InputScalar("Alpha Sort", ImGuiDataType_S16, &alpha_sort);
                        ImGui::InputScalar("FX ID", ImGuiDataType_U8, &fx_id);
                        ImGui::InputScalar("K", ImGuiDataType_S16, &K);
                        ImGui::InputScalar("L", ImGuiDataType_U8, &L);
                        ImGui::InputScalar("Animation Mode", ImGuiDataType_U8, &animation_mode);
                        ImGui::InputScalarN("DU/DV", ImGuiDataType_Float, &dudv, 2);
                        ImGui::InputScalarN("SU/SV", ImGuiDataType_Float, &susv, 2);
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                    i++;
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();

        if (ImGui::Begin("Texture Viewer"))
        {
            if (m_scene->texture_count() == 0)
            {
                ImGui::Text("No scene loaded");
            } else {
                std::vector<std::string> texture_names = m_texture_names;
                ImGui::ListBox("##LstTextures", &m_texture_selected, [](void *user_data, int which) -> const char* {
                    WelcomeView* ptr = (WelcomeView*) user_data;
                    return ptr->texture_name(which).c_str();
                }, this, m_scene->texture_count());

                if (m_texture_selected >= 0)
                {
                    ImGui::BeginChild("ImgTexture");
                    ImGui::Image((ImTextureID) m_renderer.get_texture(m_texture_selected), ImVec2(128, 128));
                    ImGui::EndChild();
                }
            }
        }
        ImGui::End();
    }
}
