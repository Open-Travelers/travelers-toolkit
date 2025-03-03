#include <imgui.h>
#include <portable-file-dialogs.h>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <vector>
#include <cstdlib>

#include "glad/glad.h"
#include "file_binary_reader.h"
#include "twoc/nu/scene.h"
#include "twoc/dat/file.h"
#include "welcome_view.h"
WelcomeView::WelcomeView(ApplicationData &app) : UI::View(app)
{

}

WelcomeView::~WelcomeView()
{
    
}

void WelcomeView::on_load(int width, int height)
{

}

UI::ViewChange WelcomeView::on_unload()
{
    return UI::ViewChange::CHANGE;
}

void WelcomeView::on_update(float dt)
{

}

void WelcomeView::on_render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load Game Root"))
            {
                std::string twoc_root = pfd::select_folder("Select extracted game root...").result();
                if (twoc_root.empty())
                {
                    (void) pfd::message("Fatal", "Can't select empty folder!", pfd::choice::ok, pfd::icon::error);
                } else {
                    if (!m_app.Project.load(twoc_root))
                    {
                        (void) pfd::message("Fatal", "Couldn't load game root!", pfd::choice::ok, pfd::icon::error);
                        exit(1);
                    }
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (m_app.Project.executable() == nullptr)
        return;

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
                            std::filesystem::path scene_path = m_app.Project.root_path() / "levels";
                            std::filesystem::path level_path(std::regex_replace(lvl.Filepath, std::regex("\\\\"), "/"));
                            level_path = level_path.make_preferred();

                            std::string level_name = level_path.filename().string();
                            level_path = level_path.remove_filename();

                            std::filesystem::path full_path = (scene_path / level_path / level_name);

                            Twoc::Nu::Scene *scene = nullptr;
                            // attempt #1 - lowercase filename
                            {
                                std::transform(level_name.begin(), level_name.end(), level_name.begin(), ::tolower);
                                full_path += ".nus";

                                FileBinaryReader reader(m_app.Project.endianness());
                                if (reader.open(full_path))
                                    scene = Twoc::Nu::Scene::from_reader(reader);
                                else
                                    std::cerr << "Lowercase path '" << full_path << "' doesn't exist!" << std::endl;
                            }

                            // attempt #2 - uppercase filename
                            if (scene == nullptr) {
                                std::transform(level_name.begin(), level_name.end(), level_name.begin(), ::toupper);
                                std::filesystem::path full_path = (scene_path / level_path / level_name);
                                full_path += ".NUS";

                                FileBinaryReader reader(m_app.Project.endianness());
                                if (reader.open(full_path))
                                    scene = Twoc::Nu::Scene::from_reader(reader);
                                else
                                    std::cerr << "Uppercase path '" << full_path << "' doesn't exist!" << std::endl;
                            }

                            if (scene == nullptr)
                            {
                                (void) pfd::message("Fatal", "Could not load scene!", pfd::choice::ok, pfd::icon::error);
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
        if (ImGui::TreeNodeEx("1#Root", ImGuiTreeNodeFlags_SpanFullWidth))
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
}
