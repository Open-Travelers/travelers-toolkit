#pragma once

#include <functional>
#include "../../ui/view.h"
#include "../../app/project.h"
#include "../../app/data.h"
#include "twoc/nu/scene.h"
#include "scene_renderer.h"
#include "camera.h"

class WelcomeView : public UI::View {
protected:
    Twoc::Nu::Scene *m_scene { nullptr };
    Twoc::Nu::Scene *m_crate_scene { nullptr };
    Twoc::Nu::Scene *m_wumpa_scene { nullptr };
    SceneRenderer m_renderer;
    Camera m_camera;
    glm::mat4 m_projection_matrix;

    bool m_rotating_camera { false };
    double m_last_mouse_x { 0 };
    double m_last_mouse_y { 0 };
    double m_mouse_dx { 0 };
    double m_mouse_dy { 0 };

    std::vector<std::string> m_texture_names;
    int m_texture_selected { -1 };

    [[nodiscard]] std::string const& texture_name(int index) const { return m_texture_names[index]; }
    bool load_scenes();
    bool load_scene(Twoc::Nu::Scene *scene);

    void on_load(int display_width, int display_height) override;
    UI::ViewChange on_unload() override;

    void on_resize(int width, int height) override;
    void on_update(float dt) override;
    void on_render() override;

    void on_mouse_press(UI::MouseButton button, double x, double y) override;
    void on_mouse_release(UI::MouseButton button, double x, double y) override;
    void on_mouse_moved(double x, double y) override;


public:
    WelcomeView(App::Data &app, std::function<bool(UI::Key)> key_held_fn);
    ~WelcomeView() override;

};